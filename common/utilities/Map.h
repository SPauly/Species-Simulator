#pragma once
#include <vector>
#include <memory>
#include "Params.h"
#include "WinConsole.h"
#include "Entity.h"
#include "TSVector.h"

namespace sim
{
    /*Abstracts the low level stuff like the checking of boundaries and displaying Entities on screen. Needs to be inhereted from!*/
    class Map
    {
    public:
        Map() = delete;
        Map(WinConsole &, params::MapConfig &, TSVector<Entity> *, std::shared_ptr<TSConsoleBuffer>);
        virtual ~Map();
        
        void start_up();
        virtual void run(bool synced = false);
        void update_single(const Entity*);
        void update_entities(bool efficiency_on = true); //efficiency_on = true -> only update those that changed
        void render();        
        void draw_line(int, int, int, int, const char&);

        std::shared_ptr<Entity> check_pos(size_t, size_t);
        params::MapConfig& get_config();
        params::WinConsoleLayout& get_layout();
        TSVector<Entity> *get_entities_vec();
        size_t get_entities_size();

    private:
        void m_draw_walls();

    protected:
        params::MapConfig m_config;
        std::shared_ptr<TSConsoleBuffer> m_buffer;
        WinConsole *mptr_console = nullptr;
        std::vector<std::shared_ptr<Entity>> m_entity_buffer;
        
    private:
        params::WinConsoleLayout m_conLay;
        std::vector<std::shared_ptr<Entity>> m_entities_internal_map;
    };
}
