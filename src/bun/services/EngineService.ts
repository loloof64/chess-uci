import { EnginePool } from "../engine/EnginePool";

export class EngineService
{
    private pool =
        new EnginePool();


    create(
        output:(id:number, line:string)=>void
    )
    {
        const id =
            this.pool.create(
                output
            );

        return id;
    }


    send(
        id:number,
        command:string
    )
    {
        this.pool
            .get(id)
            .send(command);
    }


    release(
        id:number
    )
    {
        this.pool.release(id);
    }
}