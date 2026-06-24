import {
    ApplicationMenu,
    BrowserView,
    BrowserWindow
} from "electrobun/bun";

import type { MainRPC } from "shared/rpc";

import path from "node:path";
import fs from "node:fs";
import { EngineService } from "./services/EngineService";


export function loadNativeAddon(
    name:string
)
{
    const projectRoot =
        path.resolve(
            process.cwd(),
            "../../../../"
        );


    const candidates = [
        path.join(
            projectRoot,
            "native/linux-generic",
            `${name}.node`
        ),

        path.join(
            projectRoot,
            "bin/native",
            `${name}.node`
        ),
    ];


    for(
        const candidate of candidates
    )
    {
        console.log(
            "Trying:",
            candidate
        );


        if(fs.existsSync(candidate))
        {
            console.log(
                "Loading:",
                candidate
            );

            return require(candidate);
        }
    }


    throw new Error(
        "Native addon missing:\n" +
        candidates.join("\n")
    );
}


// Keep addon loading available for StockfishNative
loadNativeAddon("stockfish");



// Engine layer
const engineService =
    new EngineService();



// HMR: use Vite dev server if running
async function getMainViewUrl():Promise<string>
{
    try
    {
        const response =
            await fetch(
                "http://localhost:5173"
            );


        if(response.ok)
        {
            return "http://localhost:5173";
        }
    }
    catch
    {
        // Vite dev server not running
    }


    return "views://mainview/index.html";
}



// Application menu
ApplicationMenu.setApplicationMenu([
    {
        submenu:[
            {
                label:"About chess-uci",
                role:"about"
            },

            {
                type:"separator"
            },

            {
                label:"Quit",
                role:"quit",
                accelerator:"q"
            },
        ],
    },

    {
        label:"Edit",

        submenu:[
            {
                role:"undo"
            },

            {
                role:"redo"
            },

            {
                type:"separator"
            },

            {
                role:"cut"
            },

            {
                role:"copy"
            },

            {
                role:"paste"
            },

            {
                role:"selectAll"
            },
        ],
    },
]);



// RPC
let mainRPC:any;


mainRPC =
    BrowserView.defineRPC<MainRPC>({
        maxRequestTime:5000,


        handlers:{

            requests:{

                ping:() =>
                {
                    return "pong";
                },


                getGreeting:() =>
                {
                    return "Greetings from the Bun side!";
                },


                createEngine:() =>
                {
                    const id =
                        engineService.create(
                            (line) =>
                            {
                                console.log(
                                    "[ENGINE]",
                                    line
                                );
                            }
                        );


                    return id;
                },



                sendEngineCommand:
                ({
                    id,
                    command
                }) =>
                {
                    engineService.send(
                        id,
                        command
                    );
                },



                releaseEngine:
                ({
                    id
                }) =>
                {
                    engineService.release(
                        id
                    );
                },

            },



            messages:{

                log:
                ({
                    msg
                }) =>
                {
                    console.log(
                        "[Webview]:",
                        msg
                    );
                },

            },

        },
    });



// Create main window
const mainWindow =
    new BrowserWindow({
        title:"chess-uci",

        url:
            await getMainViewUrl(),

        frame:{
            width:1200,
            height:800,
            x:100,
            y:100,
        },

        rpc:mainRPC,
    });



// Events
mainWindow.on(
    "close",
    () =>
    {
        console.log(
            "Main window closed"
        );

        process.exit(0);
    }
);


mainWindow.webview.on(
    "dom-ready",
    () =>
    {
        console.log(
            "Webview DOM ready"
        );
    }
);


console.log(
    "chess-uci app started"
);