#pragma once

#include <streambuf>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>


class QueueBuf : public std::streambuf
{

public:

    void push(
        const std::string& s);


protected:

    int underflow() override;


private:

    std::queue<std::string> queue;

    std::string current;


    std::mutex mutex;

    std::condition_variable cv;
};