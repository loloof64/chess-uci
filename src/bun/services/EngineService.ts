import { EnginePool } from "../engine/EnginePool";

export class EngineService {
  private pool = new EnginePool();

  create(outputCb: (line: string) => void): number {
    return this.pool.create(outputCb);
  }

  send(id: number, command: string) {
    this.pool.get(id).send(command);
  }

  release(id: number) {
    this.pool.release(id);
  }

  releaseAll() {
    this.pool.clear();
  }
}
