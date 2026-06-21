#include "streambuf.h"


void QueueBuf::push(
    const std::string& data)
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        queue.push(data + "\n");
    }

    cv.notify_one();
}



int QueueBuf::underflow()
{
    std::unique_lock<std::mutex> lock(mutex);


    cv.wait(
        lock,
        [this]
        {
            return !queue.empty();
        });


    current =
        queue.front();

    queue.pop();


    setg(
        current.data(),
        current.data(),
        current.data() + current.size());


    return traits_type::to_int_type(*gptr());
}