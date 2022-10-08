#include "ClientHandler.h"

namespace sim
{
    ClientHandler::ClientHandler(std::shared_ptr<sim::net::Connection<params::MessageType>> _connection)
        : mptr_connection(_connection)
    {
        	if(!mptr_connection->is_connected())
            {
                ERROR_FLAG = CTOR_FAIL;
                return;
            }
            
    }

    ClientHandler::~ClientHandler()
    {
        mptr_connection.reset();
    }
}