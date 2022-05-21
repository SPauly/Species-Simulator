#include "ClientMap.h"
#include <condition_variable>

namespace sim
{
    ClientMap::ClientMap(WinConsole &_console, params::MapConfig &_mapcon, TSVector<Entity> *vecptr, const std::unique_ptr<net::Connection<params::MessageType>> *ptr_connection)
        : Map(_console, _mapcon, vecptr), mptr_connection(ptr_connection)
    {
    }

    ClientMap::~ClientMap()
    {
    }

    void ClientMap::run(size_t update_freq)
    {   
        start_up();
        //main loop
        while (true)
        {
            // fetch incomming Entities
            Map::run(update_freq);
            // update screen
            mptr_console->write_buffer(mptr_console->get_active_handle(), *m_buffer);
            // read new Entity positions
            // check for connected entities -> have a deque where incomming info for displayed entities and outgoing info for their sensor inputs is stored and then distribute them here
            // send map to server
            mptr_connection->get()->send();
        }
    }
}