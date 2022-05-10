#pragma once
#include "Map.h"
#include "TSVector.h"

namespace sim
{
    class ClientMap : public Map
    {
    public:
        ClientMap(WinConsole &, params::MapConfig &, TSVector<Entity> *);
        virtual ~ClientMap();
    
        virtual void run(size_t update_freq = 1) override;
    
    private:


    };
}