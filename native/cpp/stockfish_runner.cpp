#include "stockfish_runner.h"

#include <iostream>

#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"
#include "stockfish/src/tune.h"

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

                //
                // Stockfish global initialization
                //
                Stockfish::Bitboards::init();

                Stockfish::Position::init();

                engine =
                    std::make_unique<
                        Stockfish::UCIEngine>(
                        1,
                        argv);

                Stockfish::Tune::init(
                    engine->engine_options());

                engine->loop();

                std::cin.rdbuf(oldIn);

                std::cout.rdbuf(oldOut);

                running = false;
            });
}

void StockfishRunner::send(
    const std::string &command)
{
    if (!running)
        return;

    input.rdbuf()->sputn(
        command.c_str(),
        command.size());

    input.rdbuf()->sputc('\n');
}

void StockfishRunner::stop()
{
    if (!running)
        return;

    send("quit");

    if (thread.joinable())
        thread.join();

    running = false;
}