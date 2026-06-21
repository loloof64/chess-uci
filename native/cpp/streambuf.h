#pragma once

#include <streambuf>
#include <string>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

class QueueStreamBuf : public std::streambuf
{

public:
    void push(
        const std::string &data);

protected:
    int underflow() override;

private:
    std::mutex mutex;

    std::condition_variable cv;

    std::queue<std::string> queue;

    std::string current;
};

class CallbackStreamBuf : public std::streambuf
{

public:
    using Callback =
        std::function<
            void(const std::string &)>;

    explicit CallbackStreamBuf(
        Callback callback);

protected:
    int overflow(
        int ch) override;

    int sync() override;

private:
    Callback callback;

    std::string buffer;
};