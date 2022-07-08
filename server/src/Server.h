#pragma once
#include <cstdint>
#include <thread>
#include <memory>
#include <vector>
#include "NetInclude.h"
#include "Params.h"
#include "Environment.h"
#include "WinConsole.h"
#include "Entity.h"
#include "TSVector.h"

namespace sim
{
    class Server : public net::Server_Interface<params::MessageType>
    {
    public:
        Server() = delete;
        Server(uint16_t, size_t);

        void run(size_t, bool); //arg: Max Number of Messages processed, bwait, number of Maps/Clients calls start_server, update Server and runs server

    private:    
        void mf_get_config();
        void test_console();

    public:
        virtual void on_client_validated(std::shared_ptr<net::Connection<params::MessageType>>) override;

    protected:
        virtual bool on_client_connect(std::shared_ptr<net::Connection<params::MessageType>>) override;
        virtual void on_client_disconnect(std::shared_ptr<net::Connection<params::MessageType>>) override;
        virtual void on_message(std::shared_ptr<net::Connection<params::MessageType>>, net::Message<params::MessageType> &) override;

    private:
        //window dependencies
        int x = 120;
        int y = 44;
    	WinConsole m_console{0,0,x,y,8,16};
        std::shared_ptr<TSConsoleBuffer> mptr_buffer; 

        //environment related
        size_t m_nMapsCount = DEFAULT_MAP_COUNT;
        params::MapConfig m_envConfig{0,0,0,0,params::MapType::No_Walls};
        std::unique_ptr<Environment> m_environment;
        std::thread m_EnvThread;

        //asio thread
        std::thread m_asio_update_thread;

        //shared resources
        TSVector<TSVector<Entity>> m_entities; 
        std::vector<net::TSQueue<std::shared_ptr<std::vector<Entity>>>> m_change_buffer; //buffer holds a vector of ques for each map with the incommming changes to entities
    };

}