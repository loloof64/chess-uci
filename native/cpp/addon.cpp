#include <napi.h>
#include "engine_wrapper.h"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    return EngineWrapper::Init(env, exports);
}

NODE_API_MODULE(stockfish, Init)