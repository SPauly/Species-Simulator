#pragma once
#include <vector>
#include <memory>
#include "Params.h"
#include "WinConsole.h"

namespace sim
{
    /*Abstracts the low level stuff like the checking of boundaries and displaying Entities on screen. Needs to be inhereted from!*/
    class Map
    {
    public:
        Map() = delete;
        Map(std::shared_ptr<WinConsole>, params::MapConfig &);
        Map(std::shared_ptr<WinConsole>, params::MapConfig &, std::shared_ptr<TSConsoleBuffer>);
        virtual ~Map();

        void draw_line(int, int, int, int, const char&);
        params::MapConfig& get_config();
        params::WinConsoleLayout& get_layout();

    protected:
        params::MapConfig m_config;
        std::shared_ptr<TSConsoleBuffer> m_buffer;
        std::shared_ptr<WinConsole> m_console;
    
    private:
        params::WinConsoleLayout m_conLay;
    };
}
