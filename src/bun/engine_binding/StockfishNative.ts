export type OutputCallback =
    (line:string)=>void;



export class StockfishNative
{
    private wrapper:any;


    constructor()
    {
        const addon =
            require(
                "../../../build/Release/stockfish.node"
            );


        this.wrapper =
            new addon.EngineWrapper();
    }



    start(
        callback:OutputCallback
    )
    {
        this.wrapper.start(
            callback
        );
    }



    send(
        command:string
    )
    {
        this.wrapper.send(
            command
        );
    }



    stop()
    {
        this.wrapper.stop();
    }
}