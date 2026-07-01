#include "misc.h"
#include "stockfish_runner.h"
#include "stockfish_init.h"

#include <chrono>
#include <thread>

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

    StockfishInit::init();

    input =
        std::make_unique<std::istream>(
            &inputBuffer);

    outputBuffer =
        std::make_unique<CallbackStreamBuf>(
            [this](const std::string &s)
            {
                if (callback)
                {
                    callback(s);
                }
            });

    output =
        std::make_unique<std::ostream>(
            outputBuffer.get());

    running = true;

    thread =
        std::thread(
            [this]()
            {
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

                uci->setOutputCallback(
                    [this](std::string msg)
                    {
                        (*output) << msg << "\n"
                                  << std::flush;
                    });

                uci->setStreams(
                    input.get(),
                    output.get());

                {
                    std::lock_guard<std::mutex> lock(
                        readyMutex);

                    loopReady = true;
                }

                readyCv.notify_one();

                std::string cmd;

                while (std::getline(*input, cmd))
                {
                    if (cmd.empty())
                        continue;

                    uci->execute(cmd);

                    if (cmd == "quit")
                        break;
                }
            });

    std::unique_lock<std::mutex> lock(
        readyMutex);

    readyCv.wait(
        lock,
        [this]()
        {
            return loopReady;
        });
}

void StockfishRunner::send(
    const std::string &cmd)
{
    inputBuffer.push(
        cmd + "\n");
}

void StockfishRunner::stop()
{
    if (stopping.exchange(true))
        return;

    if (running)
    {
        send("stop");
        send("quit");
    }

    if (searchThread.joinable())
        searchThread.join();

    if (thread.joinable())
        thread.join();

    running = false;
}