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

            uci->setOutputCallback(
                [this](std::string s)
                {
                    if(callback)
                        callback(s);
                });


            uci->setStreams(
                input.get(),
                nullptr
            );


            uci->loop();

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

    inputBuffer.close();


    if(thread.joinable())
        thread.join();


    uci.reset();

}
