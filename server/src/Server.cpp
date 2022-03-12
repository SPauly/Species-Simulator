#include "Server.h"

Server::Server(uint16_t nport) : sim::net::Server_Interface<sim::params::MessageType>(nport)
{
    
}

bool Server::on_client_connect(std::shared_ptr<sim::net::Connection<sim::params::MessageType>> client)
{
    sim::net::Message<sim::params::MessageType> msg;
    msg.header.id = sim::params::MessageType::Server_Accept;
    client->send(msg);
    return true;
}

void Server::on_client_disconnect(std::shared_ptr<sim::net::Connection<sim::params::MessageType>> client)
{
    std::cout << "[" << client->get_uid() << "] was removed. \n";
}

void Server::on_message(std::shared_ptr<sim::net::Connection<sim::params::MessageType>> client, sim::net::Message<sim::params::MessageType> &msg)
{
    switch (msg.header.id)
    {
    }
}
