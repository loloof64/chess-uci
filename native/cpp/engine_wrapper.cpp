#include "engine_wrapper.h"

#include "streambuf.h"

#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"


extern QueueStreamBuf inputBuffer;


Napi::FunctionReference EngineWrapper::constructor;



EngineWrapper::EngineWrapper(
    const Napi::CallbackInfo& info
)
:
Napi::ObjectWrap<EngineWrapper>(info)
{

    using namespace Stockfish;


    Bitboards::init();

    Position::init();


    char arg0[] = "stockfish";

    char* argv[] =
    {
        arg0
    };


    engine =
        std::make_unique<UCIEngine>(
            1,
            argv
        );
}



EngineWrapper::~EngineWrapper()
{

    if(engineThread.joinable())
    {
        engineThread.detach();
    }

}



Napi::Object EngineWrapper::Init(
    Napi::Env env,
    Napi::Object exports
)
{

    Napi::Function func =
        DefineClass(
            env,
            "Stockfish",
            {

                InstanceMethod(
                    "start",
                    &EngineWrapper::Start
                ),


                InstanceMethod(
                    "send",
                    &EngineWrapper::Send
                ),


                InstanceMethod(
                    "stop",
                    &EngineWrapper::Stop
                )

            }
        );


    constructor =
        Napi::Persistent(func);


    constructor.SuppressDestruct();



    exports.Set(
        "Stockfish",
        func
    );


    return exports;
}



Napi::Value EngineWrapper::Start(
    const Napi::CallbackInfo& info
)
{

    engineThread =
        std::thread(
            [this]()
            {
                engine->loop();
            }
        );


    return info.Env().Undefined();
}



Napi::Value EngineWrapper::Send(
    const Napi::CallbackInfo& info
)
{

    Napi::Env env =
        info.Env();


    if(info.Length() == 0)
        return env.Undefined();



    std::string command =
        info[0]
        .As<Napi::String>()
        .Utf8Value();



    inputBuffer.push(
        command + "\n"
    );


    return env.Undefined();
}



Napi::Value EngineWrapper::Stop(
    const Napi::CallbackInfo& info
)
{

    inputBuffer.push(
        "quit\n"
    );


    return info.Env().Undefined();
}