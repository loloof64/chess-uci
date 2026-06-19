#include "engine_wrapper.h"

#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"

Napi::FunctionReference EngineWrapper::constructor;

EngineWrapper::EngineWrapper(
    const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<EngineWrapper>(info),

      output(
          [this](const std::string &text)
          {
              Emit(text);
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

    std::cin.rdbuf(&input);

    std::cout.rdbuf(&output);
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

    engineThread =
        std::thread(
            [this]()
            {
                engine->loop();
            });

    return info.Env().Undefined();
}

Napi::Value EngineWrapper::Send(
    const Napi::CallbackInfo &info)
{

    auto env =
        info.Env();

    input.push(
        info[0]
            .As<Napi::String>()
            .Utf8Value() +
        "\n");

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
            "StockfishOutput",
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
           std::string *data)
        {
            cb.Call(
                {Napi::String::New(
                    env,
                    *data)});

            delete data;
        });
}