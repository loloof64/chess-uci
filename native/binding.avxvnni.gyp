{
  "targets": [
    {
      "target_name": "stockfish",

      "sources": [
        "cpp/addon.cpp",
        "cpp/engine_wrapper.cpp",
        "cpp/stockfish_runner.cpp",
"cpp/stockfish_init.cpp",

        "cpp/stockfish/src/benchmark.cpp",
        "cpp/stockfish/src/bitboard.cpp",
        "cpp/stockfish/src/engine.cpp",
        "cpp/stockfish/src/evaluate.cpp",
        "cpp/stockfish/src/memory.cpp",
        "cpp/stockfish/src/misc.cpp",
        "cpp/stockfish/src/movegen.cpp",
        "cpp/stockfish/src/movepick.cpp",

        "cpp/stockfish/src/nnue/features/full_threats.cpp",
        "cpp/stockfish/src/nnue/features/half_ka_v2_hm.cpp",
        "cpp/stockfish/src/nnue/network.cpp",
        "cpp/stockfish/src/nnue/nnue_accumulator.cpp",
        "cpp/stockfish/src/nnue/nnue_misc.cpp",

        "cpp/stockfish/src/position.cpp",
        "cpp/stockfish/src/score.cpp",
        "cpp/stockfish/src/search.cpp",
        "cpp/stockfish/src/syzygy/tbprobe.cpp",
        "cpp/stockfish/src/thread.cpp",
        "cpp/stockfish/src/timeman.cpp",
        "cpp/stockfish/src/tt.cpp",
        "cpp/stockfish/src/uci.cpp",
        "cpp/stockfish/src/ucioption.cpp",
        "cpp/stockfish/src/tune.cpp"
      ],

      "include_dirs": [
        "cpp/stockfish/src",
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
        "-Werror",
        "-msse",
        "-msse2",
        "-mssse3",
        "-msse3",
        "-mpopcnt",
        "-msse4.1",
        "-mavx2",
        "-mbmi",
        "-mbmi2",
        "-mavxvnni",
      ],

      "defines": [
        "NDEBUG",
        "IS_64BIT",
        "USE_POPCNT",
        "USE_SSE2",
        "USE_SSSE3",
        "USE_SSE41",
        "USE_AVX2",
        "USE_PEXT",
        "USE_VNNI",
        "USE_AVXVNNI",
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