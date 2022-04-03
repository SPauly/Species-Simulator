#include "Environment.h"

namespace sim
{
    Environment::Environment(std::shared_ptr<sim::WinConsole> _winconsole, sim::params::MapConfig &_config, int _nmaps) 
        : Map(_winconsole, _config), m_map_count(_nmaps)
    {   
    }

    Environment::~Environment()
    {}

    void Environment::instanciate_maps()
    {
        m_map_width = m_console->get_layout()._nScreenWidth / m_map_count;
        m_map_height = m_console->get_layout()._nScreenHeight / m_map_count;

        m_maps_conLayout._nScreenWidth = m_map_width;
        m_maps_conLayout._nScreenHeight = m_map_height;

        sim::params::MapConfig temp_config{0,0, m_map_width, m_map_height};
        temp_config.WallOne = sim::types::MapType::Bottom_Wall;
        temp_config.WallTwo = sim::types::MapType::Top_Wall;

        for(int i = 0; i < m_map_count; i++)
        {
            if(i % 2 > 0)
                temp_config.WallThree = sim::types::MapType::Left_Wall;
            else
                temp_config.WallThree = sim::types::MapType::Right_Wall;

            m_maps.push_back({m_console, temp_config, m_maps_conLayout});
        }
    }

    sim::params::MapConfig& Environment::at_get_config(const size_t& pos)
    {
        return m_maps.at(pos).get_config();
    }
}