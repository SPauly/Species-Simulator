#pragma once
#include "Map.h"
#include "TSVector.h"

namespace sim
{
    class ClientMap : public Map
    {
    public:
        ClientMap(WinConsole &, params::MapConfig &);
        virtual ~ClientMap();
    
        virtual void run() override;
    
    private:


    };
}