#include "Environment.h"
#include "Params.h"
#include "Rand.h"

namespace sim
{
    Environment::Environment(std::shared_ptr<WinConsole> _winconsole, params::MapConfig &_config, int _nmaps) 
        : Map(_winconsole, _config), m_map_count(_nmaps)
    {   
        m_entities.resize(m_buffer->width*m_buffer->height, nullptr);
    }

    Environment::~Environment()
    {}

    void Environment::create_entities()
    {
        /*Add food*/
        for(int i = 0; i < MAX_FOOD_PER_MAP * 2; i++)
        {
            /*generate guaranteed free x and y coordinates*/
            int randx = sim::rand(1, m_buffer->width - 1);
            int randy = sim::rand(1, m_buffer->height -1);
            while(this->check_pos(randx, randy) != nullptr)
            {
                randx = sim::rand(1, m_buffer->width - 1);
                randy = sim::rand(1, m_buffer->height -1);
            }

            m_entities.at(randy * m_buffer->width * randy) = std::make_shared<Entity>(m_id_count, randx, randy, params::EntityStyle::FOOD, params::EntityType::FOOD);
            ++m_id_count;
        }
    }
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

    std::shared_ptr<Entity> Environment::check_pos(size_t x, size_t y)
    {
        return m_entities.at(y * m_buffer->width * x);
    }

    Map& Environment::at_get_map(const size_t& pos)
    {
        return m_maps.at(pos);
    }

    size_t Environment::get_entities_size()
    {
        return m_entities.size();
    }

    std::shared_ptr<Entity>* Environment::get_entities()
    {
        return m_entities.data();
    }
}