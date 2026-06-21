#pragma once

#include <atomic>
#include <memory>
#include <thread>
#include <istream>
#include <ostream>

#include "stockfish/src/uci.h"

class StockfishRunner
{
public:
    StockfishRunner(
        std::istream &input,
        std::ostream &output);

    ~StockfishRunner();

    void start();
    void stop();

private:
    std::istream &input;
    std::ostream &output;

    std::unique_ptr<Stockfish::UCIEngine> engine;

    std::thread thread;

    std::atomic<bool> running{false};
};