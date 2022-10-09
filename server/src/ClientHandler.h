#pragma once
#include <memory>
#include <thread>
#include "NetInclude.h"
#include "Params.h"

namespace sim
{
    //flags
    static constexpr int CTOR_FAIL = 0x001;

    class ClientHandler{
    public:
        ClientHandler() = delete;
        ClientHandler(const ClientHandler&) = delete;
        ClientHandler(std::shared_ptr<sim::net::Connection<params::MessageType>>);
        virtual ~ClientHandler();

        void push_back(net::Message<params::MessageType>&);
        
    private:
        void setup();
        void run();
   
    private:
        //flags
        int ERROR_FLAG = 0x000;
        
        //functional variables
        std::shared_ptr<sim::net::Connection<params::MessageType>> mptr_connection;
        std::thread m_thread;
        net::TSQueue<net::Message<params::MessageType>> mdeq_msg{};

        //communication
        net::Message<params::MessageType> msg;
    };

}