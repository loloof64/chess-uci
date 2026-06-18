#include "engine_wrapper.h"

#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"

#include <iostream>


Napi::FunctionReference EngineWrapper::constructor;


EngineWrapper::EngineWrapper(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<EngineWrapper>(info)
{
    using namespace Stockfish;

    Bitboards::init();
    Position::init();


    engine = std::make_unique<Stockfish::Engine>();
}


EngineWrapper::~EngineWrapper()
{
    std::cerr << "EngineWrapper destroyed" << std::endl;
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