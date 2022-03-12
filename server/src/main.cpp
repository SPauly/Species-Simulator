#include <iostream>
#include "Server.h"

int main()
{
    Server server(60000);
    server.start_server();

    while (1)
    {
        server.update(-1, true);
    }
    return 0;
}