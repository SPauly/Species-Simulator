#include "Client.h"

namespace sim
{
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
                    case sim::params::MessageType::Send_Console_Layout:
                        msg >> cl;
                        m_console.create_console(cl);
                        break;
                    
                    default:
                        break;
                    }
                }
            }
        }
    }
}