#pragma once
#include <cstdint>
#include <vector>
#include "Params.h"
#include "Map.h"

namespace sim
{
    static constexpr int MAX_MAPCOUNT = 2;
    static constexpr int DEFAULT_MAP_COUNT = 2;

    class Environment : public Map
    {
    public:
        Environment(std::shared_ptr<sim::WinConsole>, sim::params::MapConfig &, int);
        virtual ~Environment();

        void instanciate_maps();
        sim::Map& at_get_map(const size_t&);

    private:
        int m_map_width = 0;
        int m_map_height = 0;

        int m_map_count = DEFAULT_MAP_COUNT;

        std::vector<sim::Map> m_maps;
    };
}
