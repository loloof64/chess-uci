#pragma once

#include <napi.h>

#include <atomic>
#include <memory>
#include <thread>

#include "streambuf.h"
#include "stockfish_runner.h"

class EngineWrapper : public Napi::ObjectWrap<EngineWrapper>
{

public:
    static Napi::Object Init(
        Napi::Env env,
        Napi::Object exports);

    EngineWrapper(
        const Napi::CallbackInfo &info);

    ~EngineWrapper();

private:
    static Napi::FunctionReference constructor;

    Napi::Value Start(
        const Napi::CallbackInfo &info);

    Napi::Value Send(
        const Napi::CallbackInfo &info);

    Napi::Value Stop(
        const Napi::CallbackInfo &info);

    Napi::Value OnOutput(
        const Napi::CallbackInfo &info);

    void Emit(
        const std::string &text);

    QueueStreamBuf inputBuffer;

    std::istream inputStream;

    CallbackStreamBuf outputBuffer;

    std::ostream outputStream;

    std::unique_ptr<StockfishRunner> runner;

    std::thread engineThread;

    std::atomic<bool> running{false};

    Napi::ThreadSafeFunction callback;
};