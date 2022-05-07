#include "Client.h"

namespace sim
{
    Client::Client(const std::string &host, const uint16_t port) 
        : net::Client_Interface<params::MessageType>(), m_host(std::move(host)), m_port(port)
    {
        m_console.create_console(0,0,60,30,8,16);
    }

    Client::~Client()
    {}


    void Client::run()
    {
        if(Client::connect_to_server(m_host, m_port))
            m_start();
        
    }

    bool Client::m_start()
    {
        //internal flags
        bool GOT_CONSOLE, GOT_MAP, GOT_ENTITIES_SIZE, GOT_ENTITIES = false;
        
        while(!M_STARTUP_OKAY)
        {
            if(this->is_connected())
            {
                if(!this->get_incomming_messages().empty())
                {
                    auto msg = get_incomming_messages().pop_front().msg;

                    switch (msg.header.id)
                    {
                    case params::MessageType::Send_Map_Console_Layout:
                        msg >> m_console_layout;
                        m_console_layout._nScreenHeight += 5; //leave space for stats
                        m_console.create_console(m_console_layout);
                        GOT_CONSOLE = true;
                        break;
                    case params::MessageType::Send_Map_Layout:
                        msg >> m_map_config;
                        m_map = std::make_unique<Map>(m_console, m_map_config);
                        m_map->start();
                        GOT_MAP = true;
                        break;
                    case params::MessageType::Send_Entities_Size:
                        msg >> m_nentities_size;
                        m_entities.resize(m_nentities_size);
                        GOT_ENTITIES_SIZE = true;
                        break; 
                    case params::MessageType::Send_Entities:
                        msg.pull_complex<Entity>(msg, m_entities.data(), m_entities.size());
                        m_map->update_entities(&m_entities);
                        GOT_ENTITIES = true;
                        if(GOT_CONSOLE && GOT_MAP && GOT_ENTITIES_SIZE && GOT_ENTITIES)
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