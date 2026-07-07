const args = process.argv.slice(2);

const mode = args.includes("--release") ? "release" : "dev";

const cpu =
  args.find((x) => x.startsWith("--cpu="))?.split("=")[1] ?? "generic";

//
// Native build
//

if (process.platform === "linux") {
  await Bun.$`
        bash scripts/build_stockfish.sh ${cpu}
    `;
}

if (process.platform === "win32") {
  await Bun.$`
        powershell scripts/build_stockfish.ps1 -Cpu ${cpu}
    `;
}

if (mode === "dev") {
  //
  // Frontend + Electrobun
  //
  // Native stockfish for ${cpu} was already built above; dev-watch.ts would
  // otherwise rebuild it again for "generic" (its own default) on startup.
  await Bun.$`
        bun run scripts/dev-watch.ts --cpu=${cpu} --skip-native-build
    `;
} else {
  await Bun.$`
        bun run build:stable
    `;
}
