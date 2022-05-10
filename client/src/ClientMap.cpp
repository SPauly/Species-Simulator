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

    void ClientMap::run(size_t update_freq)
    {
        //fetch incomming Entities
        while(true){
        Map::run(update_freq);
        //update screen
        mptr_console->write_buffer(mptr_console->get_active_handle(), *m_buffer);
        //send map to server
        //check for connected entities -> have a deque where incomming info for displayed entities and outgoing info for their sensor inputs is stored and then distribute them here
        //read new Entity positions
        }
    }
}