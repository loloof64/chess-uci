#pragma once

#include <napi.h>

#include "stockfish_runner.h"



class EngineWrapper :
public Napi::ObjectWrap<EngineWrapper>
{

public:

    static Napi::Object Init(
        Napi::Env,
        Napi::Object
    );


    EngineWrapper(
        const Napi::CallbackInfo&
    );


    ~EngineWrapper();



private:

    static Napi::FunctionReference constructor;

    bool stopped = false;


    Napi::Value Start(
        const Napi::CallbackInfo&
    );


    Napi::Value Send(
        const Napi::CallbackInfo&
    );


    Napi::Value Stop(
        const Napi::CallbackInfo&
    );


    std::unique_ptr<StockfishRunner> runner;


    Napi::ThreadSafeFunction callback;

    void stopInternal();

};