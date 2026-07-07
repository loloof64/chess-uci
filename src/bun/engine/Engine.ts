import { StockfishNative } from "../engine_binding/StockfishNative";

export class Engine {
  constructor(
    private native: StockfishNative,
    private output: (line: string) => void,
  ) {}

  start() {
    this.native.start((line: string) => {
      this.output(line);
    });
  }

  send(command: string) {
    this.native.send(command);
  }

  destroy() {
    this.native.destroy();
  }

  uci() {
    this.send("uci");
  }

  ready() {
    this.send("isready");
  }
}
