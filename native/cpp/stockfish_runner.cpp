#include "stockfish_runner.h"

#include <chrono>
#include <thread>

#include "misc.h"

StockfishRunner::StockfishRunner(
    Callback cb)
    : callback(cb)
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

    Stockfish::Bitboards::init();
    Stockfish::Position::init();

    input =
        std::make_unique<std::istream>(
            &inputBuffer);

    outputBuffer =
        std::make_unique<CallbackStreamBuf>(
            [this](const std::string &s)
            {
                std::cerr
                    << "[CALLBACK STREAM] "
                    << s;

                if (callback)
                    callback(s);
            });

    output =
        std::make_unique<std::ostream>(
            outputBuffer.get());

    running = true;

    thread =
        std::thread(
            [this]()
            {
                std::cerr << "[thread] started\n";

                int argc = 1;

                char name[] = "stockfish";

                char *argv[] =
                    {
                        name,
                        nullptr};

                uci =
                    std::make_unique<Stockfish::UCIEngine>(
                        argc,
                        argv);

                uci->setStreams(
                    input.get(),
                    output.get());

                std::cerr << "[thread] entering loop\n";

                uci->loop();
                std::cerr << "[thread] loop ended\n";
            });
}

void StockfishRunner::send(
    const std::string &cmd)
{
    std::cerr
        << "[runner] send: "
        << cmd
        << std::endl;

    inputBuffer.push(
        cmd + "\n");
}

void StockfishRunner::stop()
{
    if (stopping.exchange(true))
        return;

    std::cout << "[runner] stop() begin" << std::endl;

    if (running)
    {
        send("stop");
        send("quit");
    }

    if (thread.joinable())
        thread.join();

    running = false;

    std::cout << "[runner] stop() end" << std::endl;
}