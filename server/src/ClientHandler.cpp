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
        msg.header.receiver = 2929;
        msg << mparam_simconfig;
        mptr_connection->send(msg);
        //wait for okay from client
        wait_for_response(2929) >> CLIENT_STATUS;
        if(CLIENT_STATUS == CLIENT_READY)
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

    net::Message<params::MessageType> &ClientHandler::wait_for_response(uint32_t _key)
    {
        bool got_response = false;
 
        while(!got_response)
        {
            mdeq_msg.wait();
            if(mdeq_msg.back().header.receiver == _key || mdeq_msg.front().header.receiver == _key)
                got_response = true;
        }

        auto response = (mdeq_msg.back().header.receiver == _key) ? mdeq_msg.back() : mdeq_msg.front();
        return response;
    }
}