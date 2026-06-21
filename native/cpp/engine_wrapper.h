#pragma once

#include <napi.h>
#include <memory>
#include <thread>
#include <string>
#include <istream>
#include <ostream>

#include "streambuf.h"
#include "stockfish/src/uci.h"

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

    QueueStreamBuf input;
    QueueStreamBuf output;

    std::istream inputStream;
    std::ostream outputStream;

    std::unique_ptr<Stockfish::UCIEngine> engine;

    std::thread engineThread;

    Napi::ThreadSafeFunction outputCallback;
};