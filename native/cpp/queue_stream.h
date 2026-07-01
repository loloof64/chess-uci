#pragma once

#include <streambuf>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

class QueueStreamBuf : public std::streambuf
{
public:
    void push(const std::string &data)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);

            queue.push(data);
        }

        condition.notify_one();
    }

protected:
    int_type underflow() override
    {

        std::unique_lock<std::mutex> lock(mutex);

        condition.wait(
            lock,
            [this]
            {
                return !queue.empty();
            });

        buffer = queue.front();

        queue.pop();

        setg(
            buffer.data(),
            buffer.data(),
            buffer.data() + buffer.size());

        return traits_type::to_int_type(*gptr());
    }

    int_type uflow() override
    {
        auto c = underflow();

        if (c != traits_type::eof())
            gbump(1);

        return c;
    }

private:
    std::queue<std::string> queue;

    std::string buffer; // <-- singulier

    std::mutex mutex;

    std::condition_variable condition;
};