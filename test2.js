const sf = require("./build/Release/stockfish.node");

async function sleep(ms)
{
    return new Promise(resolve => setTimeout(resolve, ms));
}

(async () =>
{
    for(let i = 0; i < 100; i++)
    {
        console.log("engine", i);

        const e = new sf.EngineWrapper();

        e.start(line => {});

        e.send("uci");

        await sleep(50);

        e.stop();
    }

    console.log("done");
})();