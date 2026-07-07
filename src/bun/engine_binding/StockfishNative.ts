import path from "node:path";
import fs from "node:fs";

function loadNativeAddon() {
  const addonPath = path.resolve(
    __dirname,
    "../../../",
    "native/linux-generic/stockfish.node",
  );

  if (!fs.existsSync(addonPath)) {
    throw new Error("Missing addon " + addonPath);
  }

  return require(addonPath);
}

const addon = loadNativeAddon();

export class StockfishNative {
  private engine: any;

  constructor() {
    this.engine = new addon.EngineWrapper();
  }

  start(callback: (line: string) => void) {
    this.engine.start(callback);
  }

  send(command: string) {
    this.engine.send(command);
  }

  stop() {
    this.engine.stop();
  }

  destroy() {
    this.engine.stop();
    this.engine = null;
  }
}
