#pragma once
#include <cstdint>
#include <thread>
#include <memory>
#include "NetInclude.h"
#include "Params.h"
#include "Environment.h"
#include "WinConsole.h"

namespace sim
{
    class Server : public net::Server_Interface<params::MessageType>
    {
    public:
        Server(uint16_t, size_t);

        void run(size_t, bool); //arg: Max Number of Messages processed, bwait, number of Maps/Clients calls start_server, update Server and runs server

    private:    
        bool mf_start_work_thread();
        void mf_update_work();

        void mf_get_config();
        void test_console();

    public:
        virtual void on_client_validated(std::shared_ptr<net::Connection<params::MessageType>>) override;

    protected:
        virtual bool on_client_connect(std::shared_ptr<sim::net::Connection<sim::params::MessageType>>) override;
        virtual void on_client_disconnect(std::shared_ptr<sim::net::Connection<sim::params::MessageType>>) override;
        virtual void on_message(std::shared_ptr<sim::net::Connection<sim::params::MessageType>>, sim::net::Message<sim::params::MessageType> &) override;

    private:
        //window dependencies
        int x,y;
    	std::shared_ptr<sim::WinConsole> m_console;

        //work thread
        std::thread m_WorkThread;

        //environment related
        size_t m_nMapsCount = sim::DEFAULT_MAP_COUNT;
        sim::params::MapConfig m_envConfig{0,0,0,0,sim::types::MapType::No_Walls};
        std::unique_ptr<sim::Environment> m_environment;
    };

}