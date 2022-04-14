#include "Environment.h"
#include "Params.h"
#include "Rand.h"

namespace sim
{
    Environment::Environment(std::shared_ptr<WinConsole> _winconsole, params::MapConfig &_config, int _nmaps, std::vector<std::vector<Entity>> &_incomming_vec)
        : Map(_winconsole, _config), m_map_count(_nmaps)
    {
        // instantiate dimensions of each map
        m_map_width = m_config.width / m_map_count;
        m_map_height = m_config.height;

        for(int i = 0; i < m_map_count; i++)
        {
            m_incoming_entities.push_back(std::make_shared<std::vector<Entity>>(_incomming_vec.at(i)));
        }

        m_instanciate_maps();
        create_entities();
    }

    Environment::~Environment()
    {
    }

    void Environment::create_entities()
    {
        uint16_t randx = 0;
        uint16_t randy = 0; 
        Entity temp_entity;
        /*Add food to each map*/
        for (int m = 0; m < m_map_count; m++)
        {
            for (int i = 0; i < MAX_FOOD_PER_MAP; i++)
            {
                /*generate guaranteed free x and y coordinates*/
                randx = sim::rand(1, m_map_width - 1);
                randy = sim::rand(1, m_map_height - 1);
                while (m_maps.at(m).check_pos(randx, randy) != nullptr)
                {
                    randx = sim::rand(1, m_map_width - 1);
                    randy = sim::rand(1, m_map_height - 1);
                }
                temp_entity.id = m_id_count;
                temp_entity.x = randx;
                temp_entity.y = randy;
                temp_entity._char = params::EntityStyle::FOOD;
                temp_entity.type = params::EntityType::FOOD;
                m_incoming_entities.at(m)->push_back(temp_entity);
                ++m_id_count;
            }
            m_maps.at(m).update_entities(m_incoming_entities.at(m));
        }
    }
    void Environment::m_instanciate_maps()
    {
        // scale buffer to provide space for all maps
        m_buffer = std::make_shared<TSConsoleBuffer>(m_config.width, m_config.height);

        // initiate layout for the maps
        params::MapConfig temp_config{0, 0, m_map_width, m_map_height};
        temp_config.WallOne = params::MapType::Bottom_Wall;
        temp_config.WallTwo = params::MapType::Top_Wall;

        // decide on weather to draw left or right wall
        for (int i = 0; i < m_map_count; i++)
        {
            if (i % 2 > 0)
                temp_config.WallThree = params::MapType::Right_Wall;
            else
                temp_config.WallThree = params::MapType::Left_Wall;

            // set offset in Mapconfig
            temp_config.x = i * m_map_width;
            m_maps.push_back({m_console, temp_config, m_buffer});
        }
    }

    Map &Environment::at_get_map(const size_t &pos)
    {
        return m_maps.at(pos);
    }
}