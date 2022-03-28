#include "Client.h"

namespace sim
{
    void Client::run()
    {
        m_console = std::make_shared<sim::WinConsole>(0,0,60,30,8,16);
        sim::params::MapConfig map_config;
        std::shared_ptr<sim::Map> map;

        while(1)
        {
            if(this->is_connected())
            {
                if(!this->get_incomming_messages().empty())
                {
                    auto msg = get_incomming_messages().pop_front().msg;

                    switch (msg.header.id)
                    {
                    case sim::params::MessageType::Send_Console_Layout:
                        msg >> cl;
                        m_console->create_console(cl);
                        break;
                    case sim::params::MessageType::Send_Map_Layout:
                        msg >> map_config;
                        map = std::make_shared<sim::Map>(m_console, map_config, sim::params::MapType::Left_Open);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}