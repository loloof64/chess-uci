#include "stockfish_runner.h"

#include <chrono>
#include <thread>


StockfishRunner::StockfishRunner(
    Callback cb
)
:
callback(cb)
{

}


StockfishRunner::~StockfishRunner()
{
    stop();
}



void StockfishRunner::start()
{
    if(running)
        return;


    Stockfish::Bitboards::init();
    Stockfish::Position::init();

    input =
        std::make_unique<std::istream>(
            &inputBuffer
        );


    running = true;


    thread =
    std::thread(
    [this]()
    {
        std::cerr << "[thread] started\n";

        int argc = 1;

        char name[]="stockfish";

        char* argv[] =
        {
            name,
            nullptr
        };


        uci =
            std::make_unique<Stockfish::UCIEngine>(
                argc,
                argv
            );

        std::cerr << "[thread] uci created\n";


        uci->setOutputCallback(
            [this](std::string s)
            {
                std::cerr << "[SF] " << s << "\n";

                if(callback)
                    callback(s);
            });


        uci->setStreams(
            input.get(),
            nullptr
        );


        std::cerr << "[thread] entering loop\n";

        uci->loop();

        std::cerr << "[thread] loop ended\n";
    });
}



void StockfishRunner::send(
    const std::string& cmd
)
{
    inputBuffer.push(
        cmd + "\n"
    );
}





void StockfishRunner::stop()
{
    std::cerr << "[runner] stop() begin\n";

    send("stop");
    std::cerr << "[runner] sent stop\n";

    send("quit");
    std::cerr << "[runner] sent quit\n";

    if(thread.joinable())
    {
        std::cerr << "[runner] waiting thread join...\n";
        thread.join();
        std::cerr << "[runner] thread joined\n";
    }

    std::cerr << "[runner] reset uci\n";

    uci.reset();

    running = false;

    std::cerr << "[runner] stop() end\n";
}