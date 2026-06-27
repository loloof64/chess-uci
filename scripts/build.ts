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

//
// Frontend + Electrobun
//

if (mode === "dev") {
  await Bun.$`
        bun run dev:hmr
    `;

  await Bun.$`
        concurrently "bun run hmr" "bun run start"
    `;
} else {
  await Bun.$`
        bun run build:stable
    `;

  await Bun.$`
        electrobun build --env=stable
    `;
}
