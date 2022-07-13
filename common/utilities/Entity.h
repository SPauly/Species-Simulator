#pragma once
#include <cstdint>
#include "Params.h"

namespace sim
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(uint64_t, uint16_t, uint16_t, params::EntityStyle, params::EntityType);
        Entity(uint64_t, uint16_t, uint16_t, uint16_t, uint16_t, params::EntityStyle, params::EntityType);
        virtual ~Entity() = default;

        void move(int, int); //should return a vec2 with location
    public:
        uint64_t id = 0;
        uint16_t x, y = 0;
        int16_t velo_x = 0;
        int16_t velo_y = 0;
        params::EntityStyle _char = params::EntityStyle::EMPTY;
        params::EntityType type = params::EntityType::ENTITY;
    };

}