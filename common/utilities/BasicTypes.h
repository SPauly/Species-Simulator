#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <random>
#include "Params.h"

namespace sim
{
    namespace types
    {
        class rand // only temporary
        {
        public:
            rand(){};
            ~rand(){};

            int get_rand(size_t, size_t);
        };

        // type used by Map instance
        enum MapType : uint8_t
        {
            No_Walls,
            Right_Wall,
            Left_Wall,
            Top_Wall,
            Bottom_Wall,
            HAS_FILL
        };

        // Type of Entity
        enum class EntityTypes : uint8_t
        {
            ENTITY,
            GAMEOBJECT,
            FOOD,
            WALL,
            AREA,
            DEATHZONE,
            FARMZONE,
            PLAYEROBJECT
        };

        // Type of Entity Style
        enum class EntityStyle : uint8_t
        {
            EMPTY = 0;
            DOT = 46,
            HASH = 35
        };
    }
}