#ifndef STOCKFISH_RUNNER_H
#define STOCKFISH_RUNNER_H

#include <atomic>
#include <thread>
#include <memory>
#include <sstream>
#include <string>
#include <istream>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "queue_stream.h"
#include "uci.h"
#include "callback_stream.h"

class StockfishRunner
{
public:
    using Callback =
        std::function<void(const std::string &)>;

    StockfishRunner(
        Callback cb);

    ~StockfishRunner();

    void start();

    void send(
        const std::string &cmd);

    void stop();

private:
    std::atomic<bool> stopping{false};

    std::atomic<bool> running{false};

    Callback callback;

    std::thread thread;

    // Input : JS -> Stockfish
    QueueStreamBuf inputBuffer;

    std::unique_ptr<std::istream> input;

    // Output : Stockfish -> JS
    std::unique_ptr<CallbackStreamBuf> outputBuffer;

    std::unique_ptr<std::ostream> output;

    std::unique_ptr<Stockfish::UCIEngine> uci;
};

#endif