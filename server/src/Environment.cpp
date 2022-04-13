#include "Environment.h"
#include "Params.h"

namespace sim
{
    Environment::Environment(std::shared_ptr<WinConsole> _winconsole, params::MapConfig &_config, int _nmaps) 
        : Map(_winconsole, _config), m_map_count(_nmaps)
    {   
        Entity temp{};
        temp.id = 11111;
        temp.x = 1;
        temp.y = 2;
        m_entities.resize(MAX_FOOD_PER_MAP + MAX_POPULATION_PER_MAP, temp);
    }

    Environment::~Environment()
    {}

    void Environment::instanciate_maps()
    {
        //instantiate dimensions of each map
        m_map_width = m_config.width / m_map_count;
        m_map_height = m_config.height;
        
        //scale buffer to provide space for all maps
        m_buffer = std::make_shared<TSConsoleBuffer>(m_config.width, m_config.height);

        //initiate layout for the maps
        params::MapConfig temp_config{0,0, m_map_width, m_map_height};
        temp_config.WallOne = params::MapType::Bottom_Wall;
        temp_config.WallTwo = params::MapType::Top_Wall;

        //decide on weather to draw left or right wall
        for(int i = 0; i < m_map_count; i++)
        {
            if(i % 2 > 0)
                temp_config.WallThree = params::MapType::Right_Wall;
            else
                temp_config.WallThree = params::MapType::Left_Wall;

            //set offset in Mapconfig
            temp_config.x = i * m_map_width;
            m_maps.push_back({m_console, temp_config, m_buffer});
        }
    }

    Map& Environment::at_get_map(const size_t& pos)
    {
        return m_maps.at(pos);
    }

    size_t Environment::get_entities_size()
    {
        return m_entities.size();
    }

    Entity* Environment::get_entities()
    {
        return m_entities.data();
    }
}