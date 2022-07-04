#pragma once
#include <cstdint>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include "Params.h"

namespace sim
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(uint64_t, uint16_t, uint16_t, params::EntityStyle, params::EntityType);
        Entity(uint64_t, uint16_t, uint16_t, uint16_t, uint16_t, params::EntityStyle, params::EntityType);
        virtual ~Entity(){};

        Entity(const Entity &new_ent)
        {
            id = new_ent.id;
            x = new_ent.x;
            y = new_ent.y;
            velo_x = new_ent.velo_x;
            velo_y = new_ent.velo_y;
            _char = new_ent._char;
            type = new_ent.type;
        }

        Entity &operator=(const Entity &&new_ent)
        {
            id = new_ent.id;
            x = new_ent.x;
            y = new_ent.y;
            velo_x = new_ent.velo_x;
            velo_y = new_ent.velo_y;
            _char = new_ent._char;
            type = new_ent.type;
            return *this;
        }

        Entity &operator=(const Entity &new_ent)
        {
            id = new_ent.id;
            x = new_ent.x;
            y = new_ent.y;
            velo_x = new_ent.velo_x;
            velo_y = new_ent.velo_y;
            _char = new_ent._char;
            type = new_ent.type;
            return *this;
        }

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