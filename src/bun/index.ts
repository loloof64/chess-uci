import {
    ApplicationMenu,
    BrowserView,
    BrowserWindow
} from "electrobun/bun";

import type { MainRPC } from "shared/rpc";

import { EngineService } from "./services/EngineService";



// Engine layer

const engineService =
    new EngineService();



// HMR: use Vite dev server if running

async function getMainViewUrl(): Promise<string>
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
            { role:"undo" },
            { role:"redo" },

            {
                type:"separator"
            },

            { role:"cut" },
            { role:"copy" },
            { role:"paste" },
            { role:"selectAll" },
        ],
    },
]);



// RPC

const mainRPC =
    BrowserView.defineRPC<MainRPC>({
        maxRequestTime:5000,


        handlers:{

            requests:{

                createEngine: () =>
                {
                    console.log(
                        "RPC createEngine called"
                    );

                    const id =
                        engineService.create(
                            line =>
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