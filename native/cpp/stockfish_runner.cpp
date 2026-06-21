#include "stockfish_runner.h"

#include <iostream>


#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"
#include "stockfish/src/tune.h"



StockfishRunner::StockfishRunner()
{
}



StockfishRunner::~StockfishRunner()
{
    stop();
}



void StockfishRunner::start()
{

    if (running)
        return;


    running = true;


    thread =
        std::thread(
            [this]()
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



                auto old =
                    std::cin.rdbuf(
                        &input);



                engine->loop();



                std::cin.rdbuf(old);


                running = false;

            });

}



void StockfishRunner::send(
    const std::string& command)
{

    input.push(command);

}



void StockfishRunner::stop()
{

    if (running)
        send("quit");


    if (thread.joinable())
        thread.join();


    running = false;
}