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
        //small tsvec test
        std::condition_variable cv;
        TSVector<Entity> tsvec;
        tsvec.resize(10000);
        auto t = tsvec.at(2000);
        tsvec.clear();
    }
}