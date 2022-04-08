#include "Map.h"
#include <iostream>

namespace sim
{
    Map::Map(std::shared_ptr<sim::WinConsole> console, sim::params::MapConfig &config) 
        : Map(console,config, std::make_shared<sim::TSConsoleBuffer>(config.width, config.height))
    {
    }

    Map::Map(std::shared_ptr<sim::WinConsole> console, sim::params::MapConfig &config, std::shared_ptr<sim::TSConsoleBuffer> buffer)
        : m_console(console), m_config(config), m_buffer(buffer)
    {
        //set console layout
        m_conLay._nScreenWidth = config.width;
        m_conLay._nScreenHeight = config.height;
        
        //validate offset 
        if(m_config.width + m_config.x > m_console->get_layout()._nScreenWidth)
            m_config.x = m_console->get_layout()._nScreenWidth - m_config.width;
        if(m_config.height + m_config.y > m_console->get_layout()._nScreenHeight)
            m_config.y = m_console->get_layout()._nScreenHeight - m_config.height;

        //draw the walls
        switch (m_config.WallOne)
        {
        case types::MapType::No_Walls:
            break;
        case types::MapType::Right_Wall:
            draw_line(m_config.width - 1, 0, m_config.width - 1, m_config.height - 1, '|');
            break;
        case types::MapType::Left_Wall:
            draw_line(0, 0, 0, m_config.height - 1, '|');
            break;
        case types::MapType::Top_Wall:
            draw_line(0, 0, m_config.width - 1, 0, '-');
            break;
        case types::MapType::Bottom_Wall:
            draw_line(0, m_config.height - 1, m_config.width - 1, m_config.height - 1, '-');
            break;
        default:
            break;
        }

        switch (m_config.WallTwo)
        {
        case types::MapType::No_Walls:
            break;
        case types::MapType::Right_Wall:
            draw_line(m_config.width - 1, 0, m_config.width - 1, m_config.height - 1, '|');
            break;
        case types::MapType::Left_Wall:
            draw_line(0, 0, 0, m_config.height - 1, '|');
            break;
        case types::MapType::Top_Wall:
            draw_line(0, 0, m_config.width - 1, 0, '-');
            break;
        case types::MapType::Bottom_Wall:
            draw_line(0, m_config.height - 1, m_config.width - 1, m_config.height - 1, '-');
            break;
        default:
            break;
        }

        switch (m_config.WallThree)
        {
        case types::MapType::No_Walls:
            break;
        case types::MapType::Right_Wall:
            draw_line(m_config.width - 1, 0, m_config.width - 1, m_config.height - 1, '|');
            break;
        case types::MapType::Left_Wall:
            draw_line(0, 0, 0, m_config.height - 1, '|');
            break;
        case types::MapType::Top_Wall:
            draw_line(0, 0, m_config.width - 1, 0, '-');
            break;
        case types::MapType::Bottom_Wall:
            draw_line(0, m_config.height - 1, m_config.width - 1, m_config.height - 1, '-');
            break;
        default:
            break;
        }

        switch (m_config.WallFour)
        {
        case types::MapType::No_Walls:
            break;
        case types::MapType::Right_Wall:
            draw_line(m_config.width - 1, 0, m_config.width - 1, m_config.height - 1, '|');
            break;
        case types::MapType::Left_Wall:
            draw_line(0, 0, 0, m_config.height - 1, '|');
            break;
        case types::MapType::Top_Wall:
            draw_line(0, 0, m_config.width - 1, 0, '-');
            break;
        case types::MapType::Bottom_Wall:
            draw_line(0, m_config.height - 1, m_config.width - 1, m_config.height - 1, '-');
            break;
        default:
            break;
        }

        m_console->write_buffer(m_console->get_active_handle(), *m_buffer);
    }
    Map::~Map()
    {
    }

    void Map::draw_line(int _x, int _y, int _w, int _h, const char &_symb)
    {
        // make sure the starting point is closer to 0,0 than the destination point
        if (_x > _w)
            _x = _w;
        if (_y > _h)
            _y = _h;

        // make sure no diagonals are included
        if (_x != _w)
            if (_y != _h)
                _y = _h;

        // calculate difference between points
        int dif_x = _w - _x;
        int dif_y = _h - _y;

        // make sure they don't exceed boundaries
        if (_x + dif_x > m_config.width - 1)
            dif_x = m_config.width - 1 - _x;
        if (_y + dif_y > m_config.height - 1)
            dif_y = m_config.height - 1 - _y;

        //first print rows
        for (int r = 0; r <= dif_x; r++)
        {
            m_buffer->write_character((_x + r + m_config.x), _y + m_config.y, _symb);
        }
        //then columns
        for (int c = 0; c <= dif_y; c++)
        {
            m_buffer->write_character(_x + m_config.x, (_y + c + m_config.y), _symb);
        }
    }

    sim::params::MapConfig& Map::get_config()
    {
        return m_config;
    }
    sim::params::WinConsoleLayout& Map::get_layout()
    {
        return m_conLay;
    }
}