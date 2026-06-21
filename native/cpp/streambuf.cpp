#include "streambuf.h"

void QueueStreamBuf::push(const std::string &data)
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(data);
    }

    cv.notify_one();
}

int QueueStreamBuf::underflow()
{
    std::unique_lock<std::mutex> lock(mutex);

    cv.wait(
        lock,
        [this]
        {
            return !queue.empty();
        });

    current = queue.front();
    queue.pop();

    setg(
        current.data(),
        current.data(),
        current.data() + current.size());

    return traits_type::to_int_type(*gptr());
}