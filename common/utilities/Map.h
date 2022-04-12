#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include "stdarg.h"
#include "Params.h"
#include "BasicTypes.h"
#include "WinConsole.h"

namespace sim
{
    template <typename T, typename S>
    struct Entity
    {
        uint16_t id = 0;
        uint16_t x_pos = 0;
        uint16_t y_pos = 0;

        T type = 0;
        S Char = 0;
    };

    class Map
    {
    public:
        Map() = delete;
        Map(std::shared_ptr<sim::WinConsole>, sim::params::MapConfig &);
        Map(std::shared_ptr<sim::WinConsole>, sim::params::MapConfig &, std::shared_ptr<sim::TSConsoleBuffer>);
        virtual ~Map();

        void draw_line(int, int, int, int, const char&);
        sim::params::MapConfig& get_config();
        sim::params::WinConsoleLayout& get_layout();

    protected:
        sim::params::MapConfig m_config;
        std::shared_ptr<sim::TSConsoleBuffer> m_buffer;
        std::shared_ptr<sim::WinConsole> m_console;
    
    private:
        
        sim::params::WinConsoleLayout m_conLay;

        sim::types::MapType m_opening;

        std::vector<Entity<sim::types::EntityTypes, sim::types::EntityStyle>> m_vecEntities;
    };
}
