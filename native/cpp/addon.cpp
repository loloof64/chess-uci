#include <napi.h>

#include <iostream>

#include "streambuf.h"
#include "engine_wrapper.h"


QueueStreamBuf inputBuffer;


CallbackStreamBuf outputBuffer(
    [](const std::string& text)
    {
        // Temporary debug output
        printf(
            "%s",
            text.c_str()
        );
    }
);



Napi::Object Init(
    Napi::Env env,
    Napi::Object exports
)
{
    // Redirect Stockfish stdin/stdout/stderr
    std::cin.rdbuf(
        &inputBuffer
    );


    std::cout.rdbuf(
        &outputBuffer
    );


    std::cerr.rdbuf(
        &outputBuffer
    );


    return EngineWrapper::Init(
        env,
        exports
    );
}



NODE_API_MODULE(
    stockfish,
    Init
)