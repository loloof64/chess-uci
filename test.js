const sf = require("./build/Release/stockfish.node");

const e = new sf.EngineWrapper();

e.start();

e.send("uci");

setTimeout(() => {
  e.send("isready");
}, 1000);

setTimeout(() => {
  e.send("quit");
}, 3000);
