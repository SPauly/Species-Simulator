#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include "stdarg.h"
#include "Params.h"
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
        virtual ~Map();

        void draw_line(int, int, int, int, const char&);

    private:
        sim::params::MapConfig &m_config;

        sim::params::MapType m_opening;

        std::shared_ptr<sim::TSConsoleBuffer> m_buffer;

        std::shared_ptr<sim::WinConsole> m_console;

        std::vector<Entity<sim::params::EntityType, sim::params::EntityStyle>> m_vecEntities;
    };
}
