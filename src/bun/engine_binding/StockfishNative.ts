import path from "node:path";
import fs from "node:fs";

const PLATFORM_DIRS: Record<string, string> = {
  win32: "win32",
  linux: "linux",
  darwin: "darwin",
};

function findVariantDir(nativeDir: string): string {
  const platform = PLATFORM_DIRS[process.platform];
  if (!platform) {
    throw new Error(`Unsupported platform: ${process.platform}`);
  }

  const prefix = `${platform}-`;
  const candidates = fs
    .readdirSync(nativeDir, { withFileTypes: true })
    .filter((entry) => entry.isDirectory() && entry.name.startsWith(prefix))
    .map((entry) => entry.name)
    // Builds accumulate across `--cpu` switches without cleaning up older
    // variant folders, so if more than one matches, pick the most recently
    // built one rather than an arbitrary directory-listing order.
    .sort(
      (a, b) =>
        fs.statSync(path.join(nativeDir, b)).mtimeMs -
        fs.statSync(path.join(nativeDir, a)).mtimeMs,
    );

  if (candidates.length === 0) {
    throw new Error(
      `No Stockfish build found for platform "${platform}" in ${nativeDir}`,
    );
  }

  return candidates[0];
}

function loadNativeAddon() {
  const nativeDir = path.resolve(__dirname, "../../../", "native");
  const variantDir = findVariantDir(nativeDir);
  const addonPath = path.join(nativeDir, variantDir, "stockfish.node");

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
