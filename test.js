const sf = require("./build/Release/stockfish.node");


function createEngine(id)
{
    const e = new sf.EngineWrapper();


    console.log(id, "start");

    e.start();


    setTimeout(() =>
    {
        console.log(id, "uci");
        e.send("uci");

    }, 500);



    setTimeout(() =>
    {
        console.log(id, "ready");

        e.send("isready");

    }, 1500);



    setTimeout(() =>
    {
        console.log(id, "go");

        e.send("position startpos");
        e.send("go movetime 600");

    }, 2500);



    setTimeout(() =>
    {
        console.log(id, "stop");

        e.stop();

    }, 5000);


    return e;
}



const engines =
[
    createEngine("A"),
    createEngine("B"),
    createEngine("C")
];