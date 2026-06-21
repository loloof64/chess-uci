#include "streambuf.h"

void QueueStreamBuf::push(
    const std::string &data)
{

    {
        std::lock_guard<std::mutex> lock(
            mutex);

        queue.push(data);
    }

    cv.notify_one();
}

int QueueStreamBuf::underflow()
{

    std::unique_lock<std::mutex> lock(
        mutex);

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

    return traits_type::to_int_type(
        *gptr());
}

CallbackStreamBuf::CallbackStreamBuf(
    Callback cb)

    : callback(cb)
{
}

int CallbackStreamBuf::overflow(
    int ch)
{

    if (ch != EOF)
        buffer +=
            static_cast<char>(ch);

    return ch;
}

int CallbackStreamBuf::sync()
{

    if (!buffer.empty())
    {
        callback(buffer);

        buffer.clear();
    }

    return 0;
}