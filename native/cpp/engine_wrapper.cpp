#include "engine_wrapper.h"

Napi::FunctionReference
    EngineWrapper::constructor;

Napi::Object EngineWrapper::Init(
    Napi::Env env,
    Napi::Object exports)
{

    auto func =
        DefineClass(
            env,
            "EngineWrapper",
            {

                InstanceMethod(
                    "start",
                    &EngineWrapper::Start),

                InstanceMethod(
                    "send",
                    &EngineWrapper::Send),

                InstanceMethod(
                    "stop",
                    &EngineWrapper::Stop),

                InstanceMethod(
                    "onOutput",
                    &EngineWrapper::OnOutput)

            });

    constructor =
        Napi::Persistent(func);

    constructor.SuppressDestruct();

    exports.Set(
        "EngineWrapper",
        func);

    return exports;
}

EngineWrapper::EngineWrapper(
    const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<EngineWrapper>(info)
{
}

EngineWrapper::~EngineWrapper()
{
    if (runner)
        runner->stop();
}

Napi::Value EngineWrapper::Start(
    const Napi::CallbackInfo &info)
{

    runner =
        std::make_unique<StockfishRunner>(
            std::cin,
            std::cout);

    runner->start();

    return info.Env().Undefined();
}

Napi::Value EngineWrapper::Send(
    const Napi::CallbackInfo &info)
{

    if (!runner)
        return info.Env().Undefined();

    std::string cmd =
        info[0]
            .As<Napi::String>()
            .Utf8Value();

    runner->send(cmd);

    return info.Env().Undefined();
}

Napi::Value EngineWrapper::Stop(
    const Napi::CallbackInfo &info)
{

    if (runner)
        runner->stop();

    return info.Env().Undefined();
}

Napi::Value EngineWrapper::OnOutput(
    const Napi::CallbackInfo &info)
{

    callback =
        Napi::ThreadSafeFunction::New(
            info.Env(),
            info[0].As<Napi::Function>(),
            "stockfish",
            0,
            1);

    return info.Env().Undefined();
}