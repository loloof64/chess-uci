#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <string>

#include "stockfish/src/uci.h"

class StockfishRunner
{
public:
    StockfishRunner(
        std::istream &in,
        std::ostream &out);

    ~StockfishRunner();

    void start();

    void stop();

    void send(
        const std::string &command);

private:
    std::istream &input;

    std::ostream &output;

    std::unique_ptr<Stockfish::UCIEngine> engine;

    std::thread thread;

    std::atomic<bool> running =
        false;
};