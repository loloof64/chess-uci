#include "stockfish_runner.h"

#include <chrono>


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


    // Initialize Stockfish global structures once
    Stockfish::Bitboards::init();
    Stockfish::Position::init();


    input =
        std::make_unique<std::stringstream>();


    output =
        std::make_unique<std::stringstream>();


    running = true;



    engineThread =
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
                    std::make_unique<Stockfish::UCIEngine>(
                        argc,
                        argv
                    );


                uci->setStreams(
                    input.get(),
                    output.get()
                );


                // UCIEngine owns the command loop
                uci->loop();


            }
        );



    readerThread =
        std::thread(
            [this]()
            {
                while(running)
                {

                    std::string data;


                    {
                        std::lock_guard<std::mutex> lock(
                            outputMutex
                        );


                        data =
                            output->str();


                        if(!data.empty())
                        {
                            output->str("");
                            output->clear();
                        }
                    }



                    if(!data.empty() && callback)
                    {
                        callback(data);
                    }


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

    {
        std::lock_guard<std::mutex> lock(
            inputMutex
        );


        (*input)
            << cmd
            << '\n';
    }


    inputCondition.notify_one();

}
    


void StockfishRunner::stop()
{

    if(!running)
        return;


    send("quit");


    running = false;



    if(engineThread.joinable())
        engineThread.join();


    if(readerThread.joinable())
        readerThread.join();



    uci.reset();

    input.reset();
    output.reset();

}