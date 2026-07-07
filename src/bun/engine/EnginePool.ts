import { StockfishNative } from "../engine_binding/StockfishNative";
import { Engine } from "./Engine";

export type EngineId = number;

export class EnginePool {
  private engines = new Map<EngineId, Engine>();

  private nextId = 1;

  create(output: (line: string) => void): EngineId {
    const native = new StockfishNative();

    const engine = new Engine(native, output);

    const id = this.nextId++;

    this.engines.set(id, engine);

    engine.start();

    return id;
  }
  get(id: EngineId): Engine {
    const engine = this.engines.get(id);

    if (!engine) {
      throw new Error(`Unknown engine ${id}`);
    }

    return engine;
  }

  release(id: EngineId) {
    const engine = this.engines.get(id);

    if (!engine) return;

    engine.destroy();

    this.engines.delete(id);
  }

  clear() {
    for (const id of this.engines.keys()) {
      this.release(id);
    }
    this.engines.clear();
  }

  size() {
    return this.engines.size;
  }
}
