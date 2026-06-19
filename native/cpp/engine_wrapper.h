#pragma once

#include <napi.h>

#include <memory>
#include <thread>

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
        const Napi::CallbackInfo &);

    Napi::Value Send(
        const Napi::CallbackInfo &);

    Napi::Value Stop(
        const Napi::CallbackInfo &);

    Napi::Value OnOutput(
        const Napi::CallbackInfo &);

    void Emit(
        const std::string &);

private:
    std::unique_ptr<Stockfish::UCIEngine> engine;

    std::thread engineThread;

    QueueStreamBuf input;

    CallbackStreamBuf output;

    Napi::ThreadSafeFunction outputCallback;
};