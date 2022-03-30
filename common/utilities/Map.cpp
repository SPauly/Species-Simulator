#include "Map.h"
#include <iostream>

namespace sim
{
    Map::Map(std::shared_ptr<sim::WinConsole> console, sim::params::MapConfig &config) : m_console(console), m_config(config)
    {
        m_buffer = std::make_shared<sim::TSConsoleBuffer>(m_config.width, m_config.height);

        switch (m_config.WallOne)
        {
        case params::MapType::No_Walls:
            break;
        case params::MapType::Right_Wall:
        {
            // draw right
            for (int i = 0; i < m_config.height - 1; i++)
            {
                m_buffer->write_character((m_config.width - 1), i, '|');
            }
        }
        break;
        case params::MapType::Left_Wall:
        {
            // draw right
            for (int i = 0; i < m_config.height - 1; i++)
            {
                m_buffer->write_character(0, i, '|');
            }
        }
        break;
        case params::MapType::Top_Wall:
        { // draw top
            for (int i = 0; i < m_config.width; i++)
            {
                m_buffer->write_character(i, 0, '-');
            }
        }
        break;
        case params::MapType::Bottom_Wall:
        { // draw button
            for (int i = 0; i < m_config.width; i++)
            {
                m_buffer->write_character(i, (m_config.height - 1), '-');
            }
        }
        break;
        default:
            break;
        }

        m_console->write_buffer(m_console->get_active_handle(), *m_buffer);
    }

    Map::~Map()
    {
    }

    void Map::print_row(int _x, int _y, int _w, int _h, const char *_symb)
    {
        for (int i = 0; i < m_config.width; i++)
        {
            m_buffer->write_character(i, (m_config.height - 1), '-');
        }
    }
}