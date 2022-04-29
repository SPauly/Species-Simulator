#include "Entity.h"

namespace sim
{
    Entity::Entity(uint64_t _id, uint16_t _x, uint16_t _y, params::EntityStyle _style, params::EntityType _type)
        : id(_id), x(_x), y(_y), _char(_style), type(_type)
    {}
    
    Entity::Entity(uint64_t _id, uint16_t _x, uint16_t _y, uint16_t _velx, uint16_t _vely, params::EntityStyle _style, params::EntityType _type)
        : id(_id), x(_x), y(_y), velo_x(_velx), velo_y(_vely), _char(_style), type(_type)
    {}
}