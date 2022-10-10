#pragma once
#include <memory>
#include <thread>
#include "NetInclude.h"
#include "Params.h"

namespace sim
{
    //flags
    static constexpr int CTOR_FAIL = 0x001;
    static constexpr int CLIENT_READY = 0x001;
    static constexpr int CLIENT_NOT_READY = 0xFFF;
    static constexpr int CLIENT_FAIL = 0x000;

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
        
        template <typename T>
        T &wait_for_response(uint16_t, T&);
    private:
        //flags
        int ERROR_FLAG = 0x000;
        int CLIENT_STATUS = CLIENT_NOT_READY;

        //functional variables
        std::shared_ptr<sim::net::Connection<params::MessageType>> mptr_connection;
        std::thread m_thread;
        net::TSQueue<net::Message<params::MessageType>> mdeq_msg{};

        //communication
        net::Message<params::MessageType> msg;

        //simulation configuration
        params::SimConfig mparam_simconfig;
        params::WinConsoleLayout mparam_winconsolelayout;
        params::EntityConfig mparam_entityconfig;
        params::MapConfig mparam_mapconfig;
    };

}