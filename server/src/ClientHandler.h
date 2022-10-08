#pragma once
#include <memory>
#include "NetInclude.h"
#include "Params.h"

namespace sim
{
    //flags
    static constexpr int CTOR_FAIL = 0x001;

    class ClientHandler{
    public:
        ClientHandler() = delete;
        ClientHandler(std::shared_ptr<sim::net::Connection<params::MessageType>>);
        virtual ~ClientHandler();
    private:
        std::shared_ptr<sim::net::Connection<params::MessageType>> mptr_connection;
    private: //flags
        int ERROR_FLAG = 0x000;
    };

}