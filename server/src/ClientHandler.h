#pragma once
#include <memory>
#include "NetInclude.h"
#include "Params.h"

namespace sim
{
    class ClientHandler{
    public:
        ClientHandler() = delete;
        ClientHandler(std::shared_ptr<sim::net::Connection<params::MessageType>>);
        virtual ~ClientHandler();
    private:
       std::shared_ptr<sim::net::Connection<params::MessageType>> mptr_connection;
    };

}