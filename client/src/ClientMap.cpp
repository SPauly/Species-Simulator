#include "ClientMap.h"
#include "Rand.h"
#include <condition_variable>

namespace sim
{
    ClientMap::ClientMap(WinConsole &_console, params::MapConfig &_mapcon, TSVector<Entity> *vecptr, std::shared_ptr<TSConsoleBuffer> _conbuf, std::unique_ptr<net::Connection<params::MessageType>> *ptr_connection)
        : Map(_console, _mapcon, vecptr, _conbuf), mptr_connection(ptr_connection)
    {
        m_entity_message.header.id = params::MessageType::Send_Entities;
    }

    ClientMap::~ClientMap()
    {
    }

    void ClientMap::run(bool synced)
    {   
        start_up();
        //main loop
        while (true) //put condition here!
        {
            //wait for new ping from the server to start next cycle else run till end of generation
            if(synced)
                mptr_connections->wait();
            //run Neural Nets + directly update position on Map and Screen
            //testing:
            for(int i = 0; i < 99; i++)
            {
                mptr_entities_external->at_mutable(i + 500).obj.x += rand<int>(0,2);
            }
            update_entities();
            // check for connected entities -> have a deque where incomming info for displayed entities and outgoing info for their sensor inputs is stored and then distribute them here
                //send connections to server
            // send map to server
            m_entity_message.clear();
            m_entity_message.push_back_complex<Entity>(m_entity_message, mptr_entities_external->get_vector().data(), mptr_entities_external->size());
            mptr_connection->get()->send(m_entity_message);
        }
    }
}