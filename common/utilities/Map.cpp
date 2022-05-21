#include "Map.h"

namespace sim
{
    Map::Map(WinConsole &console, params::MapConfig &config, TSVector<Entity> *vecptr) 
        : Map(console,config, vecptr, std::make_shared<TSConsoleBuffer>(config.width, config.height))
    {
    }

    Map::Map(WinConsole &console, params::MapConfig &config, TSVector<Entity> *vecptr, std::shared_ptr<TSConsoleBuffer> buffer)
        : mptr_console(&console), m_config(config), mptr_entities_external(vecptr), m_buffer(buffer)
    {
        m_entities_internal_map.resize(m_config.width * m_config.height, nullptr);

        //set console layout
        m_conLay._nScreenWidth = m_config.width;
        m_conLay._nScreenHeight = m_config.height;
        
        //validate offset 
        if(m_config.width + m_config.x > mptr_console->get_layout()._nScreenWidth)
            m_config.x = mptr_console->get_layout()._nScreenWidth - m_config.width;
        if(m_config.height + m_config.y > mptr_console->get_layout()._nScreenHeight)
            m_config.y = mptr_console->get_layout()._nScreenHeight - m_config.height;
    }

    Map::~Map()
    {
    }
    
    void Map::start_up(){
        m_draw_walls();
    }

    void Map::run(size_t update_freq) //set freq. to 
    {  
        //main loop
        //wait for x number of updates in m_entities_external
        if(update_freq > mptr_entities_external->size())
            update_freq = 1;
        size_t wakeup_calls = 0;
        std::shared_ptr<std::condition_variable_any> custom_cond = std::make_shared<std::condition_variable_any>();

        while(wakeup_calls < update_freq)
        {
            mptr_entities_external->wait(custom_cond);
            ++wakeup_calls;
        }
        //update entities accourdingly
        update_entities();
        //check for necessary connections that might have to be established
    }
    
    void Map::update_entities()
    {
        update_entities(mptr_entities_external);
    }

    void Map::update_entities(TSVector<Entity> *new_entities)
    {
        mptr_entities_external = new_entities;
        
        //downside of this is many new memory allocations have to be made, upside less CPU usage since I don't have to search for anything
        for(int i = 0; i < new_entities->size(); i++)
        {
            //delete the Entity at it's previous position
            m_entities_internal_map.at((new_entities->at(i).y - new_entities->at(i).velo_y) * m_config.width + (new_entities->at(i).x - new_entities->at(i).velo_x)).reset();
            //write to new position
            m_entities_internal_map.at(new_entities->at(i).y * m_config.width + new_entities->at(i).x) = std::make_shared<Entity>(new_entities->at(i));
        }
        render(true);
    }

    void Map::render(bool WRITE_TO_BUFFER_ONLY = false)
    {
        for(int i = 0; i < m_entities_internal_map.size(); i++)
        {
            if(m_entities_internal_map.at(i))
            {
                m_buffer->write_character((m_entities_internal_map.at(i)->x + m_config.x), (m_entities_internal_map.at(i)->y + m_config.y), (char)m_entities_internal_map.at(i)->_char);
            }
        }
        if(!WRITE_TO_BUFFER_ONLY)
            mptr_console->write_buffer(mptr_console->get_active_handle(), *m_buffer);
    }

    std::shared_ptr<Entity> Map::check_pos(size_t x, size_t y)
    {
        return m_entities_internal_map.at(y * m_config.width + x);
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

    void Map::m_draw_walls()
    {
        //draw the walls
        switch (m_config.WallOne)
        {
        case params::MapType::No_Walls:
            break;
        case params::MapType::Right_Wall:
            draw_line(m_config.width - 1, 0, m_config.width - 1, m_config.height - 1, '|');
            break;
        case params::MapType::Left_Wall:
            draw_line(0, 0, 0, m_config.height - 1, '|');
            break;
        case params::MapType::Top_Wall:
            draw_line(0, 0, m_config.width - 1, 0, '-');
            break;
        case params::MapType::Bottom_Wall:
            draw_line(0, m_config.height - 1, m_config.width - 1, m_config.height - 1, '-');
            break;
        default:
            break;
        }

        switch (m_config.WallTwo)
        {
        case params::MapType::No_Walls:
            break;
        case params::MapType::Right_Wall:
            draw_line(m_config.width - 1, 0, m_config.width - 1, m_config.height - 1, '|');
            break;
        case params::MapType::Left_Wall:
            draw_line(0, 0, 0, m_config.height - 1, '|');
            break;
        case params::MapType::Top_Wall:
            draw_line(0, 0, m_config.width - 1, 0, '-');
            break;
        case params::MapType::Bottom_Wall:
            draw_line(0, m_config.height - 1, m_config.width - 1, m_config.height - 1, '-');
            break;
        default:
            break;
        }

        switch (m_config.WallThree)
        {
        case params::MapType::No_Walls:
            break;
        case params::MapType::Right_Wall:
            draw_line(m_config.width - 1, 0, m_config.width - 1, m_config.height - 1, '|');
            break;
        case params::MapType::Left_Wall:
            draw_line(0, 0, 0, m_config.height - 1, '|');
            break;
        case params::MapType::Top_Wall:
            draw_line(0, 0, m_config.width - 1, 0, '-');
            break;
        case params::MapType::Bottom_Wall:
            draw_line(0, m_config.height - 1, m_config.width - 1, m_config.height - 1, '-');
            break;
        default:
            break;
        }

        switch (m_config.WallFour)
        {
        case params::MapType::No_Walls:
            break;
        case params::MapType::Right_Wall:
            draw_line(m_config.width - 1, 0, m_config.width - 1, m_config.height - 1, '|');
            break;
        case params::MapType::Left_Wall:
            draw_line(0, 0, 0, m_config.height - 1, '|');
            break;
        case params::MapType::Top_Wall:
            draw_line(0, 0, m_config.width - 1, 0, '-');
            break;
        case params::MapType::Bottom_Wall:
            draw_line(0, m_config.height - 1, m_config.width - 1, m_config.height - 1, '-');
            break;
        default:
            break;
        }

        mptr_console->write_buffer(mptr_console->get_active_handle(), *m_buffer);
    }

    params::MapConfig& Map::get_config()
    {
        return m_config;
    }
    params::WinConsoleLayout& Map::get_layout()
    {
        return m_conLay;
    }

    TSVector<Entity> *Map::get_entities_vec()
    {
        return mptr_entities_external;
    }

    size_t Map::get_entities_size()
    {
        return mptr_entities_external->size();
    }
}