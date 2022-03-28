#include "Map.h"

namespace sim
{
    Map::Map(std::shared_ptr<sim::WinConsole> console, sim::params::MapConfig &config, sim::params::MapType open) : m_console(console), mref_config(config), m_opening(open)
    {
        m_buffer = std::make_shared<sim::TSConsoleBuffer>(mref_config.width, mref_config.height);

        //draw boundaries - testing
        //draw top
        for(int i = 0; i < mref_config.width; i++)
        {
            m_buffer->write_character(i,0, '-');
        }
        //draw right
        for(int i = 0; i < mref_config.height - 1; i++)
        {
            m_buffer->write_character((mref_config.width - 1), i, '|');
        }
        //draw button
        for(int i = 0; i < mref_config.width; i++)
        {
            m_buffer->write_character(i, (mref_config.height - 1), '-');
        }

        m_console->write_buffer(m_console->get_active_handle(), *m_buffer);
    }   

    Map::~Map()
    {
    }
}