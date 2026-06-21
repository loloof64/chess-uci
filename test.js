const sf = require("./build/Release/stockfish.node");

const e = new sf.Stockfish();

e.onOutput((line) => {
  console.log("SF>", line);
});

e.start();

setTimeout(() => {
  e.send("uci");
}, 500);

setTimeout(() => {
  e.send("isready");
}, 1500);

setTimeout(() => {
  e.send("position startpos");
  e.send("go depth 10");
}, 2500);

setTimeout(() => {
  e.send("quit");
}, 10000);
