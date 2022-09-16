#include "ClientMap.h"
#include "Rand.h"
#include <chrono>
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
        update_entities(false);
        // main loop
        while (true) // put condition here!
        {
            // wait for new ping from the server to start next cycle else run till end of generation
            if (synced)
                mptr_connections->wait();
            // run Neural Nets + directly update position on Map and Screen
            // testing:
            try
            {
                for (int i = 0; i < 5; i++)
                {
                    mptr_entities_external->at_mutable(i + 500).obj.move(rand<int>(-2, 2), rand<int>(-2, 2));
                    update_single(&mptr_entities_external->at(i + 500));
                    using namespace std::literals::chrono_literals;
                    std::this_thread::sleep_for(10ms);
                }
            }
            catch (std::out_of_range &e)
            {
            }
            // check for connected entities -> have a deque where incomming info for displayed entities and outgoing info for their sensor inputs is stored and then distribute them here
            // send connections to server
            // send map to server
            m_entity_message.clear();
            m_entity_message.push_back_complex<Entity>(m_entity_message, mptr_entities_external->get_vector().data(), mptr_entities_external->size());
            mptr_connection->get()->send(m_entity_message);
        }
    }
}