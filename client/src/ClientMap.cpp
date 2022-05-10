#include "ClientMap.h"
#include <condition_variable>

namespace sim
{
    ClientMap::ClientMap(WinConsole &_console, params::MapConfig &_mapcon, TSVector<Entity> *vecptr)
        : Map(_console, _mapcon, vecptr)
    {
    }

    ClientMap::~ClientMap()
    {
    }

    void ClientMap::run(size_t update_freq = -1)
    {
        Map::run();

        //get incomming Entities
        //update screen
        //send map to server
        //check for connected entities
        //read new Entity positions
    }
}