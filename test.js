const sf = require("./build/Release/stockfish.node");

const e = new sf.EngineWrapper();

console.log("start");

e.start();

console.log("send uci");

e.send("uci");


setTimeout(() => {
    console.log("send isready");
    e.send("isready");
}, 1000);


setTimeout(() => {
    console.log("stop");
    e.stop();
}, 5000);