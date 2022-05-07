#include "ClientMap.h"
#include <condition_variable>

namespace sim
{
    ClientMap::ClientMap(WinConsole &_console, params::MapConfig &_mapcon)
        : Map(_console, _mapcon)
    {
    }

    ClientMap::~ClientMap()
    {
    }

    void ClientMap::run()
    {
    }
}