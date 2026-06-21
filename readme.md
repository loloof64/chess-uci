# Chess Uci

A simple chess uci communication application (using Stockfish sources : I had to modify them a bit).

## For developers

### Compiling the node binding

#### Prerequesites

1. Install [bun](https://bun.com/)
2. Install Python 3
3. Install C++ tools and libraries (ex: **build-essentials** on Ubuntu, **VS studio community** on Windows 11)
4. Install Node-gyp globally (ex: `npm i -g node-gyp`)
5. Download Stockfish NNUE files defined in **native/cpp/stockfish/src/evaluate.h** from [Stockfish NNUE repository](https://github.com/official-stockfish/networks). And set the in the folder **native/cpp/stockfish/src**

##### Compilation

1. make the **scripts/build_stockfish.sh** executable if you're on linux : `chmod +x scripts/build_stockfish.sh`
2. run `.\scripts\build-stockfish.ps1 bmi2` in project root on Windows for a compilation for architecture bmi2, `./scripts/build-stockfish.sh bmi2` on Ubuntu for a compilation for architecture bmi2.
   You can change target architecture(bmi2, avx, avx2, generic).
