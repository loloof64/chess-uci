#include "stockfish_runner.h"

#include <iostream>
#include <sstream>


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


    // Same initialization as Stockfish main.cpp

    Stockfish::Bitboards::init();

    Stockfish::Position::init();



    running = true;


    thread =
        std::thread(
            [this]()
            {

                int argc = 1;

                char name[] = "stockfish";

                char* argv[] =
                {
                    name,
                    nullptr
                };


                uci =
                    std::make_unique
                    <
                        Stockfish::UCIEngine
                    >
                    (
                        argc,
                        argv
                    );


                while(running)
                {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(10)
                    );
                }

            }
        );

}



void StockfishRunner::send(
    const std::string& cmd
)
{

    /*
       For now just expose commands.
       The proper way is to redirect
       stdin/stdout like real UCI.
    */


    if(cmd=="quit")
    {
        stop();
        return;
    }


    if(callback)
    {
        callback(
            "received: " + cmd
        );
    }

}



void StockfishRunner::stop()
{

    running=false;


    if(thread.joinable())
        thread.join();


    uci.reset();

}