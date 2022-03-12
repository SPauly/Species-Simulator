#pragma once
#include <cstdint>
#include "../common/network/NetInclude.h"
#include "../common/utilities/Params.h"
#include "Environment.h"

class Server : public sim::net::Server_Interface<sim::params::MessageType>
{
public: 
    Server(uint16_t);
    virtual ~Server();

protected:  
    virtual bool on_client_connect(std::shared_ptr<sim::net::Connection<sim::params::MessageType>>);
    virtual void on_client_disconnect(std::shared_ptr<sim::net::Connection<sim::params::MessageType>>);
    virtual void on_message(std::shared_ptr<sim::net::Connection<sim::params::MessageType>>, sim::net::Message<sim::params::MessageType>);

private:    
    sim::Environment<sim::params::MapType> m_environment{};
};