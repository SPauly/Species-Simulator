#include <iostream>
#include "Server.h"

int main()
{
    sim::Server server(60000,2);
    server.run(-1, true);

    return 0;
}
