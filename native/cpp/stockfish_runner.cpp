#include "stockfish_runner.h"

#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"
#include "stockfish/src/tune.h"

#include <iostream>

StockfishRunner::StockfishRunner(
    std::istream &in,
    std::ostream &out)

    : input(in),
      output(out)
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
                //
                // Same initialization as Stockfish main.cpp
                //

                Stockfish::Bitboards::init();

                Stockfish::Position::init();

                char arg0[] = "stockfish";

                char *argv[] =
                    {
                        arg0,
                        nullptr};

                auto oldIn =
                    std::cin.rdbuf(
                        input.rdbuf());

                auto oldOut =
                    std::cout.rdbuf(
                        output.rdbuf());

                engine =
                    std::make_unique<
                        Stockfish::UCIEngine>(
                        1,
                        argv);

                /*Stockfish::Tune::init(
                    engine->engine_options());*/

                engine->loop();

                std::cin.rdbuf(oldIn);

                std::cout.rdbuf(oldOut);

                running = false;
            });
}

void StockfishRunner::stop()
{

    if (!running)
        return;

    //
    // Do not use redirected cout here.
    //
    input.clear();

    engine.reset();

    if (thread.joinable())
        thread.join();

    running = false;
}