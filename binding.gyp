{
  "targets": [
    {
      "target_name": "stockfish",

      "sources": [
        "native/cpp/addon.cpp",
        "native/cpp/engine_wrapper.cpp",
        "native/cpp/stockfish_runner.cpp",
        "native/cpp/streambuf.cpp",

        "native/cpp/stockfish/src/benchmark.cpp",
        "native/cpp/stockfish/src/bitboard.cpp",
        "native/cpp/stockfish/src/engine.cpp",
        "native/cpp/stockfish/src/evaluate.cpp",
        "native/cpp/stockfish/src/memory.cpp",
        "native/cpp/stockfish/src/misc.cpp",
        "native/cpp/stockfish/src/movegen.cpp",
        "native/cpp/stockfish/src/movepick.cpp",

        "native/cpp/stockfish/src/nnue/features/full_threats.cpp",
        "native/cpp/stockfish/src/nnue/features/half_ka_v2_hm.cpp",
        "native/cpp/stockfish/src/nnue/network.cpp",
        "native/cpp/stockfish/src/nnue/nnue_accumulator.cpp",
        "native/cpp/stockfish/src/nnue/nnue_misc.cpp",

        "native/cpp/stockfish/src/position.cpp",
        "native/cpp/stockfish/src/score.cpp",
        "native/cpp/stockfish/src/search.cpp",
        "native/cpp/stockfish/src/syzygy/tbprobe.cpp",
        "native/cpp/stockfish/src/thread.cpp",
        "native/cpp/stockfish/src/timeman.cpp",
        "native/cpp/stockfish/src/tt.cpp",
        "native/cpp/stockfish/src/uci.cpp",
        "native/cpp/stockfish/src/ucioption.cpp",
        "native/cpp/stockfish/src/tune.cpp"
      ],

      "include_dirs": [
        "native/cpp/stockfish/src",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],

      "dependencies": [
        "<!@(node -p \"require('node-addon-api').gyp\")"
      ],

      "cflags": [
        "-pthread"
      ],

      "cflags_cc": [
        "-std=c++20",
        "-pthread",
        "-O3",
        "-fexceptions",
        "-Werror"
      ],

      "defines": [
        "NDEBUG",
        "IS_64BIT",
        "NODE_ADDON_API_CPP_EXCEPTIONS"
      ],

      "ldflags": [
        "-pthread"
      ],

      "msvs_settings": {
        "VCCLCompilerTool": {
          "AdditionalOptions": [
            "/std:c++20",
            "/O2"
          ]
        }
      }
    }
  ]
}