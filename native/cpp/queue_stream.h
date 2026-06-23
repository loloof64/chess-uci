#pragma once

#include <streambuf>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>


class QueueStreamBuf : public std::streambuf
{
public:

    void push(
        const std::string& data
    )
    {
        {
            std::lock_guard<std::mutex> lock(mutex);

            queue.push(data);
        }

        condition.notify_one();
    }

    void close()
    {
        push("quit\n");
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
            }
        );


        buffer = queue.front();

        queue.pop();


        setg(
            buffer.data(),
            buffer.data(),
            buffer.data() + buffer.size()
        );


        return traits_type::to_int_type(*gptr());
    }


private:

    std::queue<std::string> queue;

    std::string buffer;

    std::mutex mutex;

    std::condition_variable condition;
};