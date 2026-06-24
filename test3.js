const sf =
    require("./build/Release/stockfish.node");


function sleep(ms)
{
    return new Promise(r => setTimeout(r, ms));
}


async function test(id)
{
    const e =
        new sf.EngineWrapper();


    e.start(
        line =>
        {
            console.log(
                `[${id}]`,
                line
            );
        }
    );


    await sleep(200);


    e.send("uci");

    await sleep(200);


    e.send("isready");

    await sleep(200);


    console.log(id, "start search");


    e.send(
        "position startpos"
    );


    e.send(
        "go movetime 5000"
    );


    // Let Stockfish think
    await sleep(500);


    console.log(id, "STOP DURING SEARCH");


    e.stop();


    console.log(id, "stopped");


    return e;
}


(async () =>
{
    const engines = [];


    for(let i = 0; i < 10; i++)
    {
        engines.push(
            await test(
                "E" + i
            )
        );
    }


    console.log("all stopped");


    await sleep(1000);


    console.log("done");

})();