const args = process.argv.slice(2);

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

await Bun.$`
      bun run build:stable
  `;
