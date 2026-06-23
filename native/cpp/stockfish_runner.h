#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <condition_variable>

#include "stockfish/src/uci.h"
#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"


class StockfishRunner
{
public:

    using Callback =
        std::function<void(const std::string&)>;


    explicit StockfishRunner(
        Callback cb
    );


    ~StockfishRunner();


    void start();


    void stop();


    void send(
        const std::string& cmd
    );


private:

    void outputLoop();


private:

    Callback callback;


    std::thread engineThread;
    std::thread readerThread;


    std::atomic<bool> running{false};


    std::unique_ptr<Stockfish::UCIEngine> uci;


    std::unique_ptr<std::stringstream> input;
    std::unique_ptr<std::stringstream> output;


    std::mutex inputMutex;
    std::condition_variable inputCondition;


    std::mutex outputMutex;
};