// test.js
const path = require("path");

const addonPath = path.resolve(__dirname, "build", "Release", "stockfish.node");

const sf = require(addonPath);

console.log("Addon loaded");

const engine = new sf.Stockfish();

engine.onOutput((line) => {
  console.log("SF>", line);
});

engine.start();

setTimeout(() => {
  console.log("send uci");
  engine.send("uci");
}, 1000);

setTimeout(() => {
  console.log("send isready");
  engine.send("isready");
}, 3000);

setTimeout(() => {
  console.log("send quit");
  engine.send("quit");
}, 5000);
