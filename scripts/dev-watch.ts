import { watch } from "node:fs";

// electrobun's own `dev --watch` only kills its launcher process on rebuild,
// not the grandchild bun runtime process the launcher spawns. That grandchild
// keeps the native stockfish.node addon mapped in memory. Electrobun then
// overwrites stockfish.node in place while rebuilding the asar, so the next
// launch can dlopen a torn file (or racy state) and segfault. Driving the
// process ourselves lets us fully kill the process group and confirm death
// before rebuilding.

const args = process.argv.slice(2);
const cpu = args.find((x) => x.startsWith("--cpu="))?.split("=")[1] ?? "generic";
const skipNativeBuild = args.includes("--skip-native-build");

const WATCH_DIRS = ["src"];
const DEBOUNCE_MS = 300;

let child: ReturnType<typeof Bun.spawn> | null = null;
let building = false;
let rebuildPending = false;
let debounceTimer: Timer | null = null;
let shuttingDown = false;

async function killChildTree() {
  if (!child) return;
  const pid = child.pid;

  if (process.platform === "win32") {
    // No process groups / setsid on Windows — ask taskkill to tear down the
    // whole tree (launcher + grandchild bun runtime holding stockfish.node).
    Bun.spawnSync(["taskkill", "/PID", String(pid), "/T", "/F"]);
    await child.exited.catch(() => {});
    child = null;
    return;
  }

  // `setsid` made `pid` a session/process-group leader, so -pid targets the
  // whole tree it spawned (launcher + grandchild bun runtime holding stockfish.node).
  try {
    process.kill(-pid, "SIGTERM");
  } catch {}

  const deadline = Date.now() + 3000;
  while (Date.now() < deadline) {
    try {
      process.kill(-pid, 0); // throws once the group is gone
    } catch {
      break;
    }
    await Bun.sleep(50);
  }

  try {
    process.kill(-pid, "SIGKILL");
  } catch {}

  await child.exited.catch(() => {});
  child = null;
}

async function buildNativeStockfish() {
  if (skipNativeBuild) return;
  if (process.platform === "linux") {
    await Bun.$`bash scripts/build_stockfish.sh ${cpu}`;
  } else if (process.platform === "win32") {
    await Bun.$`powershell scripts/build_stockfish.ps1 -Cpu ${cpu}`;
  }
}

async function buildFrontend() {
  const result = Bun.spawnSync(["bun", "run", "vite", "build"], {
    stdio: ["inherit", "inherit", "inherit"],
  });
  if (result.exitCode !== 0) {
    throw new Error(`vite build failed with exit code ${result.exitCode}`);
  }
}

async function buildAndLaunch() {
  await buildFrontend();

  // `electrobun dev` (no --watch) builds the app bundle and launches it in
  // one shot — this is the path that works reliably (see `bun run start`).
  // On non-Windows, setsid puts the whole subtree (launcher + grandchild bun
  // runtime) into a fresh session/process group whose leader pid == child.pid,
  // so we can reliably tear it all down with a single process.kill(-pid, ...).
  // On Windows there's no such thing, so we spawn directly and tear the tree
  // down with taskkill /T instead (see killChildTree).
  const cmd =
    process.platform === "win32"
      ? ["bunx", "electrobun", "dev"]
      : ["setsid", "bunx", "electrobun", "dev"];
  const spawned = Bun.spawn(cmd, {
    stdio: ["inherit", "inherit", "inherit"],
  });
  child = spawned;

  // If the app window is closed by the user (not by our own rebuild-triggered
  // kill), the app process exits on its own — follow it and quit the watcher too.
  spawned.exited.then(() => {
    if (child === spawned && !building && !shuttingDown) {
      shutdown();
    }
  });
}

async function rebuild(reason: string) {
  if (building) {
    rebuildPending = true;
    return;
  }
  building = true;
  rebuildPending = false;

  console.log(`\n[dev-watch] ${reason} — rebuilding...\n`);

  await killChildTree();

  try {
    await buildAndLaunch();
  } catch (err) {
    console.error("[dev-watch] build failed:", err);
  }

  building = false;
  if (rebuildPending && !shuttingDown) {
    await rebuild("pending changes");
  }
}

function scheduleRebuild(reason: string) {
  if (shuttingDown) return;
  if (debounceTimer) clearTimeout(debounceTimer);
  debounceTimer = setTimeout(() => rebuild(reason), DEBOUNCE_MS);
}

async function shutdown() {
  if (shuttingDown) return;
  shuttingDown = true;
  console.log("\n[dev-watch] Shutting down...");
  if (debounceTimer) clearTimeout(debounceTimer);
  await killChildTree();
  process.exit(0);
}

process.on("SIGINT", shutdown);
process.on("SIGTERM", shutdown);

for (const dir of WATCH_DIRS) {
  watch(dir, { recursive: true }, (_event, filename) => {
    scheduleRebuild(filename ? `${dir}/${filename} changed` : `${dir} changed`);
  });
}

await buildNativeStockfish();
await rebuild("initial build");
