#include "ClientHandler.h"

namespace sim
{
    ClientHandler::ClientHandler(std::shared_ptr<sim::net::Connection<params::MessageType>> _connection)
        : mptr_connection(_connection)
    {
        if (!mptr_connection->is_connected())
        {
            ERROR_FLAG = CTOR_FAIL;
            return;
        }
        
        m_thread = std::thread([this]()
        {   
            setup();
        });
    }

    ClientHandler::~ClientHandler()
    {
        mptr_connection.reset();
        if(m_thread.joinable())
            m_thread.join();
    }

    void ClientHandler::setup()
    {
        //send all the config stuff to clients and wait for go from client
        msg.header.id = params::MessageType::Send_SimConfig;
        run();
    }

    void ClientHandler::run()
    {
        while(true) //add flag here
        {
            mdeq_msg.wait();

            while(!mdeq_msg.empty())
            {

            }
        }
    }

    void ClientHandler::push_back(net::Message<params::MessageType> &_msg)
    {
        mdeq_msg.push_back(_msg);
    }
}