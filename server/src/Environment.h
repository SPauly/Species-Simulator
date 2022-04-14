#pragma once
#include <cstdint>
#include <vector>
#include "Params.h"
#include "Map.h"
#include "Entity.h"

namespace sim
{
    /*Config values are only temporarily stored like this*/
    static constexpr int MAX_MAPCOUNT = 2;
    static constexpr int DEFAULT_MAP_COUNT = 2;
    static constexpr int MAX_POPULATION_PER_MAP = 100;
    static constexpr int MAX_FOOD_PER_MAP = 200;

    class Environment : public Map
    {
    public:
        Environment(std::shared_ptr<WinConsole>, params::MapConfig &, int);
        virtual ~Environment();

        void create_entities();

        std::shared_ptr<Entity> check_pos(size_t,size_t);

        Map& at_get_map(const size_t&);

    private:
        void m_instanciate_maps();

    private:
        int m_map_width = 0;
        int m_map_height = 0;

        int m_map_count = DEFAULT_MAP_COUNT;
        uint64_t m_id_count = 0;

        std::vector<Map> m_maps;

        std::vector<std::shared_ptr<std::vector<Entity>>> m_incoming_entities; // A vector of shared_ptrs to vectors of Entities -> used to hold different pointers to different vectors of incomming Entities
    };
}
