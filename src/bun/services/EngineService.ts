import {
    EnginePool,
    EngineId
} from "../engine/EnginePool";


export class EngineService
{
    private pool =
        new EnginePool();



    create()
    {
        const id =
            this.pool.create(
                line =>
                {
                    // TODO: broadcast RPC
                    console.log(
                        "[engine]",
                        id,
                        line
                    );
                }
            );


        return id;
    }



    send(
        id:EngineId,
        command:string
    )
    {
        this.pool
            .get(id)
            .send(command);
    }



    release(
        id:EngineId
    )
    {
        this.pool.release(id);
    }
}