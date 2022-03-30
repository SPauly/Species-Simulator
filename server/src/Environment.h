#pragma once
#include <cstdint>
#include <vector>
#include "Params.h"
#include "BasicTypes.h"
#include "Map.h"

namespace sim
{
    static constexpr int MAX_MAPCOUNT = 9;
    static constexpr int DEFAULT_MAP_COUNT = 2;

    class Environment 
    {
    public:
        Environment();
        virtual ~Environment();

    private:
        int m_map_width = 0;
        int m_map_height = 0;

        int m_map_count = DEFAULT_MAP_COUNT;


        std::vector<sim::Map> m_maps;
    };
}
