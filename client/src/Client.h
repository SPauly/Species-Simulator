#pragma once
#include <vector>
#include "NetInclude.h"
#include "Params.h"
#include "WinConsole.h"
#include "Map.h"
#include "Entity.h"

namespace sim
{
    class Client : public net::Client_Interface<params::MessageType>
    {
    public:
        Client();
        virtual ~Client();
        
        void run();
    private:
        std::shared_ptr<WinConsole> m_console;
        params::WinConsoleLayout m_console_layout;

        params::MapConfig m_map_config;
        std::shared_ptr<sim::Map> m_map;

        size_t m_nentities_size = 0;
        std::vector<Entity> m_entities;
    };
}