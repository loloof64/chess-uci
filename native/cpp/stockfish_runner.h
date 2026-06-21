#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "stockfish/src/uci.h"


class QueueBuf : public std::streambuf
{

public:

    void push(
        const std::string& value);


protected:

    int underflow() override;


private:

    std::mutex mutex;

    std::condition_variable cv;

    std::queue<std::string> queue;

    std::string current;

};



class StockfishRunner
{

public:

    StockfishRunner();

    ~StockfishRunner();


    void start();

    void stop();


    void send(
        const std::string& command);



private:

    QueueBuf input;


    std::unique_ptr<
        Stockfish::UCIEngine>
        engine;


    std::thread thread;


    std::atomic<bool>
        running{false};

};