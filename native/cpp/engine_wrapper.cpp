#include "engine_wrapper.h"

Napi::FunctionReference EngineWrapper::constructor;

Napi::Object EngineWrapper::Init(
    Napi::Env env,
    Napi::Object exports)
{

    auto clazz =
        DefineClass(
            env,
            "Stockfish",
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
        Napi::Persistent(clazz);

    constructor.SuppressDestruct();

    exports.Set(
        "Stockfish",
        clazz);

    return exports;
}

EngineWrapper::EngineWrapper(
    const Napi::CallbackInfo &info)

    : Napi::ObjectWrap<EngineWrapper>(info),

      inputStream(&inputBuffer),

      outputBuffer(
          [this](const std::string &text)
          {
              Emit(text);
          }),

      outputStream(&outputBuffer)
{
}

EngineWrapper::~EngineWrapper()
{

    if (runner)
    {
        inputBuffer.push(
            "quit\n");
    }

    if (engineThread.joinable())
    {
        engineThread.join();
    }

    running = false;

    if (callback)
    {
        callback.Release();
    }
}

Napi::Value EngineWrapper::Start(
    const Napi::CallbackInfo &info)
{

    auto env =
        info.Env();

    if (running)
        return env.Undefined();

    running = true;

    engineThread =
        std::thread(
            [this]()
            {
                runner =
                    std::make_unique<
                        StockfishRunner>(
                        inputStream,
                        outputStream);

                runner->start();

                if (runner)
                {
                    runner->stop();
                }

                running = false;
            });

    return env.Undefined();
}

Napi::Value EngineWrapper::Send(
    const Napi::CallbackInfo &info)
{

    if (!runner)
        return info.Env().Undefined();

    auto cmd =
        info[0]
            .As<Napi::String>()
            .Utf8Value();

    inputBuffer.push(
        cmd + "\n");

    return info.Env().Undefined();
}

Napi::Value EngineWrapper::Stop(
    const Napi::CallbackInfo &info)
{

    if (runner)
    {
        inputBuffer.push(
            "quit\n");
    }

    if (engineThread.joinable())
    {
        engineThread.join();
    }

    running = false;

    return info.Env().Undefined();
}

Napi::Value EngineWrapper::OnOutput(
    const Napi::CallbackInfo &info)
{

    callback =
        Napi::ThreadSafeFunction::New(
            info.Env(),

            info[0]
                .As<Napi::Function>(),

            "stockfish-output",

            0,

            1);

    return info.Env().Undefined();
}

void EngineWrapper::Emit(
    const std::string &text)
{

    if (!callback)
        return;

    callback.BlockingCall(
        [text](
            Napi::Env env,
            Napi::Function fn)
        {
            fn.Call(
                {Napi::String::New(
                    env,
                    text)});
        });
}