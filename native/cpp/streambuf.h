#pragma once

#include <streambuf>
#include <string>
#include <queue>
#include <mutex>
#include <functional>

class CallbackStreamBuf : public std::streambuf
{
public:
    using Callback =
        std::function<void(const std::string &)>;

    explicit CallbackStreamBuf(
        Callback cb);

protected:
    int overflow(
        int ch) override;

private:
    Callback callback_;
    std::string buffer_;
};

class QueueStreamBuf : public std::streambuf
{
public:
    void push(
        const std::string &data);

protected:
    int underflow() override;

private:
    std::queue<std::string> queue_;
    std::string current_;
    std::mutex mutex_;
};