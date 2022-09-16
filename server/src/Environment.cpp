#include "Environment.h"
#include "Params.h"
#include "Rand.h"

namespace sim
{
    Environment::Environment(WinConsole &_winconsole, params::MapConfig &_config, std::shared_ptr<TSConsoleBuffer> _conbuf, int _nmaps, TSVector<TSVector<Entity>> *_incomming_vec, std::vector<net::TSQueue<std::shared_ptr<std::vector<Entity>>>> *_change_buffer)
        : Map(_winconsole, _config, nullptr, _conbuf), m_map_count(_nmaps), mptr_change_buffer(_change_buffer)
    {
        // instantiate dimensions of each map
        m_map_width = m_config.width / m_map_count;
        m_map_height = m_config.height;

        mptr_entities = _incomming_vec;
    }

    Environment::~Environment()
    {
        bRUNNING = false;
    }

    void Environment::run(bool synced)
    {
        start_up();
        m_instanciate_maps();
        m_create_entities();

        bRUNNING = true;

        // start threads to update Maps asynchronously
        for (int i = 0; i < m_maps.size(); i++)
        {
            m_mapThreads.push_back(std::thread([this, i, synced]()
                                               {
                std::shared_ptr<std::vector<Entity>> ptr_tmp_buf;
                m_maps.at(i).start_up();
                try
                {
                    while (bRUNNING)
                    {
                        mptr_change_buffer->at(i).wait();
                        while (!mptr_change_buffer->at(i).empty())
                        {
                            ptr_tmp_buf = mptr_change_buffer->at(i).pop_front();
                            ptr_tmp_buf->at(0).x = 10;
                            // iterate through first element -> implement in vector of all entities
                            // update entities on screen
                            m_maps.at(i).update_entities();
                            // send incomming connections to main distribution queue
                        }
                    }
                }
                catch (std::out_of_range &e)
                {
                } }));
        }

        // join map threads
        for (int i = 0; i < m_maps.size(); i++)
        {
            if (m_mapThreads.at(i).joinable())
                m_mapThreads.at(i).join();
        }
    }

    void Environment::m_create_entities()
    {
        uint16_t randx = 0;
        uint16_t randy = 0;
        Entity temp_entity;
        /*Add food to each map*/
        for (int map_counter = 0; map_counter < mptr_entities->size(); map_counter++)
        {
            for (int i = 0; i < MAX_FOOD_PER_MAP; i++)
            {
                randx = sim::rand(1, m_map_width - 1);
                randy = sim::rand(1, m_map_height - 1);

                temp_entity.id = m_id_count;
                temp_entity.x = randx;
                temp_entity.y = randy;
                temp_entity._char = params::EntityStyle::FOOD;
                temp_entity.type = params::EntityType::FOOD;

                mptr_entities->at(map_counter).push_back(temp_entity);
                ++m_id_count;
            }
            m_maps.at(map_counter).update_entities(false);
        }
        /*Add Players to each map*/
        for (int map_counter = 0; map_counter < mptr_entities->size(); map_counter++)
        {
            for (int i = 0; i < MAX_POPULATION_PER_MAP; i++)
            {
                randx = sim::rand(1, m_map_width - 1);
                randy = sim::rand(1, m_map_height - 1);

                temp_entity.id = m_id_count;
                temp_entity.x = randx;
                temp_entity.y = randy;
                temp_entity._char = params::EntityStyle::PLAYER;
                temp_entity.type = params::EntityType::PLAYEROBJECT;

                mptr_entities->at(map_counter).push_back(temp_entity);
                ++m_id_count;
            }
            m_maps.at(map_counter).update_entities(false);
        }
    }

    void Environment::m_instanciate_maps()
    {
        // initiate layout for the maps
        params::MapConfig temp_config{0, 0, m_map_width, m_map_height};
        temp_config.WallOne = params::MapType::Bottom_Wall;
        temp_config.WallTwo = params::MapType::Top_Wall;

        // decide on wether to draw left or right wall
        for (int i = 0; i < m_map_count; i++)
        {
            if (i % 2 > 0)
                temp_config.WallThree = params::MapType::Right_Wall;
            else
                temp_config.WallThree = params::MapType::Left_Wall;

            // set offset in Mapconfig
            temp_config.x = i * m_map_width;
            m_maps.push_back({*mptr_console, temp_config, &mptr_entities->at(i), m_buffer});
        }
    }

    Map &Environment::at_get_map(const size_t &pos)
    {
        return m_maps.at(pos);
    }
}