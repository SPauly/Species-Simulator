#pragma once
#include <cstdint>
#include "BasicTypes.h"

namespace sim 
{
    class Entity
    {
    public: 
        Entity();
        virtual ~Entity();

    protected:
        uint64_t id = 0;
        uint16_t x,y,w,h = 0;
        types::EntityStyle _char = types::EntityStyle::EMPTY;
        types::EntityTypes type = types::EntityTypes::ENTITY;
    };

}