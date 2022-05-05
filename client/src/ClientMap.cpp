#include "ClientMap.h"
#include <utilities>

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
        //small tsvec test
        std::condition_variable cv;
        TSVector<Entity> tsvec(cv);
        tsvec.resize(10000);
        tsvec.at(2000) = (Entity){0,0};
        tsvec.clear();
    }
}