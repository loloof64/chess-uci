#include "engine_wrapper.h"

#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"

Napi::FunctionReference EngineWrapper::constructor;

EngineWrapper::EngineWrapper(
    const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<EngineWrapper>(info),
      output(
          [this](const std::string &s)
          {
              Emit(s);
          })
{

    using namespace Stockfish;

    Bitboards::init();
    Position::init();

    char arg0[] = "stockfish";

    char *argv[] =
        {
            arg0};

    engine =
        std::make_unique<UCIEngine>(
            1,
            argv);
}

EngineWrapper::~EngineWrapper()
{
    if (engineThread.joinable())
        engineThread.detach();
}

Napi::Object EngineWrapper::Init(
    Napi::Env env,
    Napi::Object exports)
{

    auto func =
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
        Napi::Persistent(func);

    constructor.SuppressDestruct();

    exports.Set(
        "Stockfish",
        func);

    return exports;
}

Napi::Value EngineWrapper::Start(
    const Napi::CallbackInfo &info)
{

    auto env = info.Env();

    std::cin.rdbuf(
        &input);

    std::cout.rdbuf(
        &output);

    std::cerr.rdbuf(
        &output);

    engineThread =
        std::thread(
            [this]()
            {
                engine->loop();
            });

    return env.Undefined();
}

Napi::Value EngineWrapper::Send(
    const Napi::CallbackInfo &info)
{

    auto env = info.Env();

    if (info.Length() == 0)
        return env.Undefined();

    std::string cmd =
        info[0]
            .As<Napi::String>()
            .Utf8Value();

    input.push(
        cmd + "\n");

    return env.Undefined();
}

Napi::Value EngineWrapper::Stop(
    const Napi::CallbackInfo &info)
{

    input.push(
        "quit\n");

    return info.Env().Undefined();
}

Napi::Value EngineWrapper::OnOutput(
    const Napi::CallbackInfo &info)
{

    outputCallback =
        Napi::ThreadSafeFunction::New(
            info.Env(),
            info[0].As<Napi::Function>(),
            "stockfish-output",
            0,
            1);

    return info.Env().Undefined();
}

void EngineWrapper::Emit(
    const std::string &text)
{

    if (!outputCallback)
        return;

    auto *msg =
        new std::string(text);

    outputCallback.BlockingCall(
        msg,
        [](Napi::Env env,
           Napi::Function cb,
           std::string *value)
        {
            cb.Call(
                {Napi::String::New(
                    env,
                    *value)});

            delete value;
        });
}