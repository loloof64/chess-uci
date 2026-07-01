#include "engine_wrapper.h"

Napi::FunctionReference
    EngineWrapper::constructor;

Napi::Object EngineWrapper::Init(
    Napi::Env env,
    Napi::Object exports)
{

    auto cls =
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
                    &EngineWrapper::Stop)

            });

    constructor =
        Napi::Persistent(cls);

    constructor.SuppressDestruct();

    exports.Set(
        "EngineWrapper",
        cls);

    return exports;
}

EngineWrapper::EngineWrapper(
    const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<EngineWrapper>(info)
{
}

EngineWrapper::~EngineWrapper()
{
    stopInternal();
}

Napi::Value EngineWrapper::Start(
    const Napi::CallbackInfo &info)
{

    if (runner)
        stopInternal();

    stopped = false;

    auto env =
        info.Env();

    if (info.Length() == 0 ||
        !info[0].IsFunction())
    {
        return env.Undefined();
    }

    callback =
        Napi::ThreadSafeFunction::New(
            env,
            info[0].As<Napi::Function>(),
            "stockfish",
            0,
            1);

    runner =
        std::make_unique<StockfishRunner>(
            [this](const std::string &line)
            {
                if (stopped)
                    return;

                auto *txt =
                    new std::string(line);

                callback.NonBlockingCall(
                    txt,
                    [](
                        Napi::Env env,
                        Napi::Function fn,
                        std::string *value)
                    {
                        fn.Call(
                            {Napi::String::New(
                                env,
                                *value)});

                        delete value;
                    });
            });

    runner->start();

    return env.Undefined();
}

Napi::Value EngineWrapper::Send(
    const Napi::CallbackInfo &info)
{

    if (!runner)
        return info.Env().Undefined();

    runner->send(
        info[0]
            .As<Napi::String>()
            .Utf8Value());

    return info.Env().Undefined();
}

Napi::Value EngineWrapper::Stop(
    const Napi::CallbackInfo &info)
{
    stopInternal();
    return info.Env().Undefined();
}

void EngineWrapper::stopInternal()
{
    if (stopped)
        return;

    stopped = true;

    if (runner)
    {
        runner->stop();
        runner.reset();
    }

    if (callback)
    {
        callback.Release();
        callback = {};
    }
}