#include "stockfish_runner.h"


#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"
#include "stockfish/src/tune.h"
#include "stockfish/src/uci.h"


#include <iostream>


StockfishRunner::StockfishRunner()
{
}



StockfishRunner::~StockfishRunner()
{
    stop();
}



void StockfishRunner::start(
    OutputCallback cb)
{

    if(running)
        return;


    callback = cb;


    running = true;


    thread =
        std::thread(
            &StockfishRunner::run,
            this);
}



void StockfishRunner::run()
{

    char arg0[] = "stockfish";


    char* argv[] =
    {
        arg0,
        nullptr
    };


    Stockfish::Bitboards::init();

    Stockfish::Position::init();


    engine =
        std::make_unique<
            Stockfish::UCIEngine>(
                1,
                argv);



    Stockfish::Tune::init(
        engine->engine_options());



    auto oldIn =
        std::cin.rdbuf(
            &inputBuffer);


    auto oldOut =
        std::cout.rdbuf(
            &outputBuffer);



    engine->loop();



    std::cin.rdbuf(oldIn);

    std::cout.rdbuf(oldOut);



    running=false;
}



void StockfishRunner::send(
    const std::string& command)
{

    inputBuffer.push(command);
}



void StockfishRunner::stop()
{

    if(!running)
        return;


    send("quit");


    if(thread.joinable())
        thread.join();


    running=false;
}   