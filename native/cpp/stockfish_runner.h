#pragma once

#include <atomic>
#include <thread>
#include <functional>
#include <memory>
#include <string>

#include "streambuf.h"

namespace Stockfish {
class UCIEngine;
}

class StockfishRunner
{
public:

    using OutputCallback =
        std::function<void(const std::string&)>;


    StockfishRunner();

    ~StockfishRunner();


    void start(OutputCallback cb);

    void send(
        const std::string& command);


    void stop();


private:

    void run();


private:

    std::atomic<bool> running{false};

    std::thread thread;


    std::unique_ptr<Stockfish::UCIEngine> engine;


    QueueBuf inputBuffer;
    QueueBuf outputBuffer;


    OutputCallback callback;
};