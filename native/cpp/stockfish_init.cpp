#include "stockfish_init.h"

#include <mutex>

#include "stockfish/src/bitboard.h"
#include "stockfish/src/position.h"


namespace
{
    std::once_flag flag;
}


namespace StockfishInit
{

void init()
{
    std::call_once(
        flag,
        []
        {
            Stockfish::Bitboards::init();
            Stockfish::Position::init();
        });
}

}