#include "engine_wrapper.h"

#include <iostream>


Napi::FunctionReference EngineWrapper::constructor;


EngineWrapper::EngineWrapper(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<EngineWrapper>(info)
{
    std::cerr << "Creating engine START" << std::endl;

    engine = std::make_unique<Stockfish::Engine>();

    std::cerr << "Creating engine END" << std::endl;
}


EngineWrapper::~EngineWrapper()
{
    std::cout << "EngineWrapper destroyed" << std::endl;
}


Napi::Object EngineWrapper::Init(
    Napi::Env env,
    Napi::Object exports
)
{
    Napi::Function func = DefineClass(
        env,
        "Stockfish",
        {
            InstanceMethod(
                "go",
                &EngineWrapper::Go
            ),

            InstanceMethod(
                "stop",
                &EngineWrapper::Stop
            )
        }
    );

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set(
        "Stockfish",
        func
    );

    return exports;
}


Napi::Value EngineWrapper::Go(
    const Napi::CallbackInfo& info
)
{
    return info.Env().Undefined();
}


Napi::Value EngineWrapper::Stop(
    const Napi::CallbackInfo& info
)
{
    return info.Env().Undefined();
}