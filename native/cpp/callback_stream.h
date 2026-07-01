#pragma once

#include <streambuf>
#include <functional>
#include <string>
#include <iostream>

class CallbackStreamBuf : public std::streambuf
{
public:
    using Callback =
        std::function<void(const std::string &)>;

    CallbackStreamBuf(
        Callback cb)
        : callback(cb)
    {
    }

protected:
    int overflow(
        int ch) override
    {
        if (ch != EOF)
        {
            buffer.push_back(
                static_cast<char>(ch));

            if (ch == '\n')
            {
                flush();
            }
        }

        return ch;
    }

    int sync() override
    {
        flush();
        return 0;
    }

private:
    void flush()
    {

        if (buffer.empty())
            return;

        if (callback)
            callback(buffer);

        buffer.clear();
    }

    std::string buffer;

    Callback callback;
};