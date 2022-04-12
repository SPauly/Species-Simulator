#pragma once
#include <cstdint>
#include "Params.h"

namespace sim 
{
    class Entity
    {
    public: 
        Entity() = default;
        virtual ~Entity(){};

    public:
        uint64_t id = 0;
        uint16_t x,y = 0;
        params::EntityStyle _char = params::EntityStyle::EMPTY;
        params::EntityType type = params::EntityType::ENTITY;
    };

}