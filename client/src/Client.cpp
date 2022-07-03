#include "Client.h"
#include "Rand.h"

namespace sim
{
    Client::Client(const std::string &host, const uint16_t port)
        : net::Client_Interface<params::MessageType>(), m_host(std::move(host)), m_port(port)
    {
        m_console.create_console(0, 0, 60, 30, 8, 16);
    }

    Client::~Client()
    {
        if (m_mapThread.joinable())
            m_mapThread.join();
        if (m_asio_update_thread.joinable())
            m_asio_update_thread.join();
        disconnect();
    }

    void Client::run()
    {
        if (Client::connect_to_server(m_host, m_port))
            m_start();

        // start map thread
        m_mapThread = std::thread([this]()
                                  { if(m_map) 
                                        m_map->run(); });


        // prime asio thread with message distribution work
        m_asio_update_thread = std::thread([this]()
        { while (true)
        {
            update(-1, true); // runs the asio loop -> processes incomming messages
        }
        });

        //prime main thread with 30fps rendering
        while(true)
        {
            mptr_console_buffer->write_buffer_to_console(&m_console);
        }
    }

    void Client::on_message(net::Message<params::MessageType> &msg)
    {
        switch (msg.header.id)
        {
        case params::MessageType::Send_Entities:
            msg.pull_complex<Entity>(msg, m_ent_buffer.data(), m_nentities_size);
            m_entities = m_ent_buffer; // = operator overloaded to wakeup m_mapThread
            break;
        }
    }

    bool Client::m_start()
    {
        // internal flags
        bool GOT_CONSOLE, GOT_MAP, GOT_ENTITIES_SIZE, GOT_ENTITIES = false;

        while (!M_STARTUP_OKAY)
        {
            if (this->is_connected())
            {
                if (!this->get_incomming_messages().empty())
                {
                    auto msg = get_incomming_messages().pop_front().msg;

                    switch (msg.header.id)
                    {
                    case params::MessageType::Send_Map_Console_Layout:
                        msg >> m_console_layout;
                        m_console_layout._nScreenHeight += 5; // leave space for stats
                        m_console.create_console(m_console_layout);
                        mptr_console_buffer = std::make_shared<TSConsoleBuffer>(m_console_layout._nScreenWidth, m_console_layout._nScreenHeight);
                        GOT_CONSOLE = true;
                        break;
                    case params::MessageType::Send_Map_Layout:
                        msg >> m_map_config;
                        m_map = std::make_unique<ClientMap>(m_console, m_map_config, &m_entities, mptr_console_buffer, &m_uptrConnection);
                        GOT_MAP = true;
                        break;
                    case params::MessageType::Send_Entities_Size:
                        msg >> m_nentities_size;
                        m_entities.resize(m_nentities_size);
                        m_ent_buffer.resize(m_nentities_size);
                        GOT_ENTITIES_SIZE = true;
                        break;
                    case params::MessageType::Send_Entities:
                        msg.pull_complex<Entity>(msg, m_ent_buffer.data(), m_nentities_size);
                        m_entities = m_ent_buffer;
                        m_map->update_entities(&m_entities);
                        GOT_ENTITIES = true;
                        if (GOT_CONSOLE && GOT_MAP && GOT_ENTITIES_SIZE && GOT_ENTITIES)
                            M_STARTUP_OKAY = true;
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        return M_STARTUP_OKAY = true;
    }
}