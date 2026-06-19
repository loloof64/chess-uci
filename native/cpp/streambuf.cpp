#include "streambuf.h"

CallbackStreamBuf::CallbackStreamBuf(
    Callback cb)
    : callback_(cb)
{
}

int CallbackStreamBuf::overflow(
    int ch)
{
    if (ch == EOF)
        return EOF;

    buffer_ += char(ch);

    if (ch == '\n')
    {
        callback_(buffer_);
        buffer_.clear();
    }

    return ch;
}

void QueueStreamBuf::push(
    const std::string &data)
{

    {
        std::lock_guard lock(
            mutex_);

        queue_.push(
            data);
    }

    cv_.notify_one();
}

int QueueStreamBuf::underflow()
{

    std::unique_lock lock(
        mutex_);

    cv_.wait(
        lock,
        [this]()
        {
            return !queue_.empty();
        });

    current_ =
        queue_.front();

    queue_.pop();

    setg(
        current_.data(),
        current_.data(),
        current_.data() + current_.size());

    return traits_type::to_int_type(
        *gptr());
}