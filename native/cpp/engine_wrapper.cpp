#include "engine_wrapper.h"


Napi::FunctionReference EngineWrapper::constructor;



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
                    &EngineWrapper::Stop)

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
    const Napi::CallbackInfo& info)
:
Napi::ObjectWrap<EngineWrapper>(info)
{
}




EngineWrapper::~EngineWrapper()
{
    if (runner)
        runner->stop();
}




Napi::Value EngineWrapper::Start(
const Napi::CallbackInfo& info)
{

    auto env =
        info.Env();


    if(!runner)
    {
        runner =
            std::make_unique<StockfishRunner>();
    }


    runner->start(
        [env,this](const std::string& text)
        {

            if(callback)
            {
                auto* msg =
                    new std::string(text);


                callback.BlockingCall(
                    msg,
                    [](Napi::Env env,
                       Napi::Function js,
                       std::string* value)
                    {
                        js.Call(
                        {
                            Napi::String::New(
                                env,
                                *value)
                        });


                        delete value;
                    });
            }

        });


    return env.Undefined();
}


Napi::Value EngineWrapper::Send(
    const Napi::CallbackInfo& info)
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
    const Napi::CallbackInfo& info)
{

    if (runner)
        runner->stop();



    return info.Env().Undefined();
}