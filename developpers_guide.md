# For developers

## Compiling the node binding

### Prerequesites

1. Install [bun](https://bun.com/)
2. Install Python 3
3. Install C++ tools and libraries (ex: **build-essentials** on Ubuntu, **VS studio community** on Windows 11)
4. Install Node-gyp globally (ex: `npm i -g node-gyp`)
5. Download Stockfish NNUE files defined in **native/cpp/stockfish/src/evaluate.h** from [Stockfish NNUE repository](https://github.com/official-stockfish/networks). And set the in the folder **native/cpp/stockfish/src**
6. Install vite.js : ``
7. Install Bun dependencies for the project : `bun install`

#### Compilation

- Available processors : generic (default) / avx / avx2 / bmi2
- Targets : debug (default) / release

- Examples :
  `bun run scripts/build.ts --cpu=avx2`(dev mode/ avx2 processors)
  `bun run build --release --cpu=avx2` (release mode / avx2 processors)
