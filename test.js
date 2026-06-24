const sf =
require("./build/Release/stockfish.node");


function createEngine(id)
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


    console.log(id,"start");


    setTimeout(()=>{
        console.log(id,"uci");
        e.send("uci");
    },500);



    setTimeout(()=>{
        console.log(id,"ready");
        e.send("isready");
    },1500);



    setTimeout(()=>{
        console.log(id,"go");

        e.send(
          "position startpos"
        );

        e.send(
          "go movetime 600"
        );

    },2500);



    setTimeout(()=>{
        console.log(id,"stop");

        e.stop();
        e.stop();

    },5000);


}


createEngine("A");
createEngine("B");
createEngine("C");