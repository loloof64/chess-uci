import { StockfishNative } from "../engine_binding/StockfishNative";


export class Engine
{
    constructor(
        private native:StockfishNative,
        private output:(line:string)=>void
    )
    {
    }


    start()
    {
        this.native.start(
            this.output
        );
    }


    send(
        command:string
    )
    {
        this.native.send(
            command
        );
    }


    stop()
    {
        this.native.stop();
    }


    uci()
    {
        this.send("uci");
    }


    ready()
    {
        this.send("isready");
    }
}