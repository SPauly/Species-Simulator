#pragma once
#include <vector>
#include <memory>
#include "Params.h"
#include "WinConsole.h"
#include "Entity.h"

namespace sim
{
    /*Abstracts the low level stuff like the checking of boundaries and displaying Entities on screen. Needs to be inhereted from!*/
    class Map
    {
    public:
        Map() = delete;
        Map(WinConsole &, params::MapConfig &);
        Map(WinConsole &, params::MapConfig &, std::shared_ptr<TSConsoleBuffer>);
        virtual ~Map();
        
        void start_map();
        void update_entities(std::vector<Entity>*);
        void render();
        
        void draw_line(int, int, int, int, const char&);

        std::shared_ptr<Entity> check_pos(size_t, size_t);
        params::MapConfig& get_config();
        params::WinConsoleLayout& get_layout();
        std::vector<Entity> *get_entities_vec();
        size_t get_entities_size();

    protected:
        params::MapConfig m_config;
        std::shared_ptr<TSConsoleBuffer> m_buffer;
        WinConsole *mptr_console = nullptr;
    
    private:
        params::WinConsoleLayout m_conLay;
        std::vector<Entity> *mptr_entities_external;
        std::vector<std::shared_ptr<Entity>> m_entities_internal_map;
    };
}
