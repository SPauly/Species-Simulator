#include "Entity.h"

namespace sim
{
    Entity::Entity(uint64_t _id, uint16_t _x, uint16_t _y, params::EntityStyle _style, params::EntityType _type)
        : id(id), x(_x), y(_y), _char(_style), type(_type)
    {}
}