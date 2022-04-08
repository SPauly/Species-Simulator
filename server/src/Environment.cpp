#include "Environment.h"
#include "BasicTypes.h"

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
        //instantiate dimensions of each map
        m_map_width = m_config.width / m_map_count;
        m_map_height = m_config.height;
        
        //scale buffer to provide space for all maps
        m_buffer = std::make_shared<sim::TSConsoleBuffer>(m_config.width, m_config.height);

        //initiate layout for the maps
        sim::params::MapConfig temp_config{0,0, m_map_width, m_map_height};
        temp_config.WallOne = sim::types::MapType::Bottom_Wall;
        temp_config.WallTwo = sim::types::MapType::Top_Wall;

        //decide on weather to draw left or right wall
        for(int i = 0; i < m_map_count; i++)
        {
            if(i % 2 > 0)
                temp_config.WallThree = sim::types::MapType::Right_Wall;
            else
                temp_config.WallThree = sim::types::MapType::Left_Wall;

            //set offset in Mapconfig
            temp_config.x = i * m_map_width;
            m_maps.push_back({m_console, temp_config, m_buffer});
        }
    }

    sim::Map& Environment::at_get_map(const size_t& pos)
    {
        return m_maps.at(pos);
    }
}