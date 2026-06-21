#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <string_view>

#include "engine.h"
#include "search.h"

namespace Stockfish
{

    class Position;
    class Move;
    class Score;

    enum Square : uint8_t;

    using Value = int;

    class UCIEngine
    {
    public:
        UCIEngine(int argc, char **argv);

        static UCIEngine &active();

        static std::ostream &active_output();

        void setStreams(
            std::istream *in,
            std::ostream *out);

        void loop();

        static int to_cp(Value v, const Position &pos);

        static std::string format_score(const Score &s);

        static std::string square(Square s);

        static std::string move(
            Move m,
            bool chess960);

        static std::string wdl(
            Value v,
            const Position &pos);

        static std::string to_lower(
            std::string str);

        static Move to_move(
            const Position &pos,
            std::string str);

        static Search::LimitsType parse_limits(
            std::istream &is);

        auto &engine_options()
        {
            return engine.get_options();
        }

    private:
        Engine engine;

        CommandLine cli;

        static thread_local UCIEngine *current;

        // Instance-specific UCI streams
        std::istream *inputStream = &std::cin;

        std::ostream *outputStream = &std::cout;

        void print_info_string(
            std::string_view str);

        void go(
            std::istringstream &is);

        void bench(
            std::istream &args);

        void benchmark(
            std::istream &args);

        void position(
            std::istringstream &is);

        void setoption(
            std::istringstream &is);

        std::uint64_t perft(
            const Search::LimitsType &);

        void on_update_no_moves(
            const Engine::InfoShort &info);

        void on_update_full(
            const Engine::InfoFull &info,
            bool showWDL);

        void on_iter(
            const Engine::InfoIter &info);

        void on_bestmove(
            std::string_view bestmove,
            std::string_view ponder);

        void init_search_update_listeners();
    };

} // namespace Stockfish

#endif
