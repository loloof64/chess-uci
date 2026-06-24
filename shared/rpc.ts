// shared/rpc.ts

import type { RPCSchema } from "electrobun";


export type MainRPC = RPCSchema<{
    requests: {

        createEngine: {
            params: Record<string, never>;
            response: number;
        };


        sendEngineCommand: {
            params: {
                id:number;
                command:string;
            };

            response:void;
        };


        releaseEngine: {
            params:{
                id:number;
            };

            response:void;
        };

    };


    messages: {

        log:{
            msg:string;
        };


        engineOutput:{
            id:number;
            line:string;
        };

    };
}>;



export type WebviewRPC = RPCSchema<{
    requests: Record<string, never>;

    messages: Record<string, never>;
}>;