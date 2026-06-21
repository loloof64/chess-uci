#pragma once

#include <streambuf>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>


class QueueStreamBuf : public std::streambuf
{

public:

    void push(
        const std::string& data);


protected:

    int underflow() override;


private:

    std::mutex mutex;

    std::condition_variable cv;

    std::queue<std::string> queue;

    std::string current;
};