#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>

class keyobj{
    public:
        keyobj(std::string key, std::string value);
        std::string key{};
        std::string value{};
};

// class hardware{
//     public:
//         hardware();
//         std::vector<keyobj> keys;
// }

class bot{

    public:
        std::string unique_name{};
        std::string ip{};
        int port{};
        int fd{};
        bool isAlive{};
        std::vector< std::string > words{};
};

class server
{
    public:
        int sfd{};
        int port{};
        std::string host{};
        std::vector< bot * > bots;
        server(std::string host, int port);
        void add(std::string ip, int port, int fd);
        bool setup();
        void handler();
        void cleanup();
        void brodcast(std::string message);
};

#endif