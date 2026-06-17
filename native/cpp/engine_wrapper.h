#pragma once

#include <napi.h>
#include <memory>

#include "stockfish/src/engine.h"

class EngineWrapper : public Napi::ObjectWrap<EngineWrapper>
{
public:
    static Napi::Object Init(
        Napi::Env env,
        Napi::Object exports
    );

    EngineWrapper(
        const Napi::CallbackInfo& info
    );

    ~EngineWrapper();

private:
    static Napi::FunctionReference constructor;

    Napi::Value Go(
        const Napi::CallbackInfo& info
    );

    Napi::Value Stop(
        const Napi::CallbackInfo& info
    );

    std::unique_ptr<Stockfish::Engine> engine;
};