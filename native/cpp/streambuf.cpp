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
    std::lock_guard<std::mutex> lock(
        mutex_);

    queue_.push(data);
}

int QueueStreamBuf::underflow()
{
    std::lock_guard<std::mutex> lock(
        mutex_);

    if (queue_.empty())
        return EOF;

    current_ =
        queue_.front();

    queue_.pop();

    setg(
        current_.data(),
        current_.data(),
        current_.data() + current_.size());

    return *gptr();
}