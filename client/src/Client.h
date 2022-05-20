#pragma once
#include <vector>
#include <memory>
#include "NetInclude.h"
#include "Params.h"
#include "WinConsole.h"
#include "Map.h"
#include "Entity.h"
#include "TSVector.h"
#include "ClientMap.h"

namespace sim
{
    class Client : public net::Client_Interface<params::MessageType>
    {
    public:
        Client() = delete;
        Client(const std::string &host, const uint16_t port);
        virtual ~Client();
        
        void run();
    
    protected:
        virtual void on_message(net::Message<params::MessageType> &msg) override;
    
    private:
        bool m_start();

    private:
        //FLAGS
        bool M_STARTUP_OKAY = false; //set to true when everything was initialized properly
        
        //Connection details
        const std::string m_host;
        const uint16_t m_port;

        //Console 
        params::WinConsoleLayout m_console_layout;  
        WinConsole m_console{m_console_layout};

        //Map
        std::thread m_mapThread; 
        params::MapConfig m_map_config;
        std::unique_ptr<sim::ClientMap> m_map;
        std::vector<Entity> m_ent_buffer;

        //shared resources
        size_t m_nentities_size = 0;
        TSVector<Entity> m_entities;
    };
}