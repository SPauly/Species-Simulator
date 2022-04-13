#include "Client.h"

namespace sim
{
    Client::Client() : net::Client_Interface<params::MessageType>()
    {
        m_console = std::make_shared<WinConsole>(0,0,60,30,8,16);
    }
    Client::~Client()
    {}

    void Client::run()
    {
        while(1)
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
                        m_console->create_console(m_console_layout);
                        break;
                    case params::MessageType::Send_Map_Layout:
                        msg >> m_map_config;
                        m_map = std::make_shared<Map>(m_console, m_map_config);
                        break;
                    case params::MessageType::Send_Entities_Size:
                        msg >> m_nentities_size;
                        m_entities.resize(m_nentities_size);
                        break; 
                    case params::MessageType::Send_Entities:
                        msg.pull_complex<Entity>(msg, m_entities.data(), m_entities.size());
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}