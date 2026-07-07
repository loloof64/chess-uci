# For developers

## Compiling the node binding

### Prerequesites

1. Install [bun](https://bun.com/)
2. Install Python 3
3. Install C++ tools and libraries (ex: **build-essentials** on Ubuntu, **VS studio community** on Windows 11)
4. Install Node-gyp globally (ex: `bun i -g node-gyp`)
5. Download Stockfish NNUE files defined in **native/cpp/stockfish/src/evaluate.h** from [Stockfish NNUE repository](https://github.com/official-stockfish/networks). And set the in the folder **native/cpp/stockfish/src**
6. Install Bun dependencies for the project : `bun install`

#### Compilation

##### Available processors

| Processor (`--cpu`) | Description                             | CPU compatibility                                                 |
| ------------------- | --------------------------------------- | ----------------------------------------------------------------- |
| `avx-512icl`        | Fastest                                 | Intel (Ice Lake, Rocket Lake and newer) and AMD (Zen 4 and newer) |
| `vnni512`           | A bit slower than AVX-512ICL            | Intel (Cascade Lake and newer with AVX512-VNNI)                   |
| `avx512`            | A bit slower than VNNI-512              | Intel (Skylake-X, Cascade Lake-X)                                 |
| `avxvnni`           | A bit faster than BMI2                  | Intel (Alder Lake and newer)                                      |
| `bmi2`              | A bit faster than AVX2                  | Intel (2013+) and AMD Zen 3 (2020+)                               |
| `avx2`              | Recommended for most users              | Intel (2013+) and AMD (2015+)                                     |
| `popcnt`            | A bit slower than AVX2                  | Intel (2008+) and AMD (2011+)                                     |
| `generic`           | Very slow, but works on all 64-bit CPUs | All 64-bit processors                                             |

##### Available targets

- debug (dev script)
- release (build script)

##### Examples

`bun run dev --cpu=avx2`(dev mode/ avx2 processors)
`bun run build --cpu=avx2` (release mode / avx2 processors)
