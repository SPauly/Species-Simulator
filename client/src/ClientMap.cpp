#include "ClientMap.h"
#include <condition_variable>

namespace sim
{
    ClientMap::ClientMap(WinConsole &_console, params::MapConfig &_mapcon, TSVector<Entity> *vecptr, std::unique_ptr<net::Connection<params::MessageType>> *ptr_connection)
        : Map(_console, _mapcon, vecptr), mptr_connection(ptr_connection)
    {
        m_entity_message.header.id = params::MessageType::Send_Entities;
    }

    ClientMap::~ClientMap()
    {
    }

    void ClientMap::run(size_t update_freq)
    {   
        start_up();
        //main loop
        while (true) //put condition here!
        {
            // fetch incomming Entities every update_freq changes to m_entities_external
            Map::run(update_freq);
            // update screen
            m_buffer->write_buffer_to_console(mptr_console);
            // check for connected entities -> have a deque where incomming info for displayed entities and outgoing info for their sensor inputs is stored and then distribute them here
                //send connections to server
            // send map to server
            m_entity_message.push_back_complex<Entity>(m_entity_message, mptr_entities_external->get_vector().data(), mptr_entities_external->size());
            mptr_connection->get()->send(m_entity_message);
        }
    }
}