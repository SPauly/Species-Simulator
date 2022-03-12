#pragma once
#include <cstdint>
#include "../common/network/NetInclude.h"
#include "../common/utilities/Params.h"
#include "Environment.h"

class Server : public sim::net::Server_Interface<sim::params::MessageType>
{
public: 
    Server(uint16_t);
    ~Server();


}