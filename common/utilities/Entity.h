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
        virtual ~Entity(){};

    public:
        uint64_t id = 0;
        uint16_t x,y = 0;
        params::EntityStyle _char = params::EntityStyle::EMPTY;
        params::EntityType type = params::EntityType::ENTITY;
    };

}