#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <string>
#include <memory>

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

    void loop();


private:

    Callback callback;


    std::thread thread;


    std::atomic<bool> running{false};


    std::unique_ptr<Stockfish::UCIEngine> uci;


};