#include <iostream>
#include "NetInclude.h"

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class CustomServer : public sim::net::Server_Interface<CustomMsgTypes>
{
public:
    CustomServer(uint16_t nport) : sim::net::Server_Interface<CustomMsgTypes>(nport)
    {
    }

protected:
    virtual bool on_client_connect(std::shared_ptr<sim::net::Connection<CustomMsgTypes>> client)
    {
        sim::net::Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerAccept;
        client->send(msg);
        return true;
    }

    virtual void on_client_disconnect(std::shared_ptr<sim::net::Connection<CustomMsgTypes>> client)
    {
        std::cout << "[" << client->get_uid() << "] ... Client gets removed\n";
    }

    virtual void on_message(std::shared_ptr<sim::net::Connection<CustomMsgTypes>> client, sim::net::Message<CustomMsgTypes> &msg)
    {
        switch (msg.header.id)
        {
        case CustomMsgTypes::ServerPing:
        {
            std::cout << "[" << client->get_uid() << "] Server Ping\n";
            client->send(msg);
        }
        break;
        case CustomMsgTypes::MessageAll:
		{
			std::cout << "[" << client->get_uid() << "]: Message All\n";

			// Construct a new message and send it to all clients
			sim::net::Message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::ServerMessage;
			msg << client->get_uid();
			message_all_clients(msg, client);

		}
		break;
        }
    }
};

int main()
{
    CustomServer server(60000);
    server.start_server();

    while (1)
    {
        server.update(-1, true);
    }
    return 0;
}