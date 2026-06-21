#pragma once

#include <thread>
#include <atomic>
#include <string>

#include "streambuf.h"

class StockfishRunner
{

public:
    StockfishRunner(
        std::function<void(const std::string &)>);

    ~StockfishRunner();

    void start();

    void stop();

    void send(
        const std::string &);

private:
    void loop();

private:
    QueueStreamBuf input;

    CallbackStreamBuf output;

    std::istream in;

    std::ostream out;

    std::thread thread;

    std::atomic<bool> running{false};
};