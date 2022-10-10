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
        msg.header.request_id = 2929;
        msg << mparam_simconfig;
        mptr_connection->send(msg);
        //wait for okay from client
        wait_for_response<int>(2929, CLIENT_STATUS);
        
        if(CLIENT_STATUS != CLIENT_FAIL)
            run();
    }

    void ClientHandler::run()
    {
        while(true) //add flag here
        {
            mdeq_msg.wait();

            while(!mdeq_msg.empty())
            {
                auto current_msg = mdeq_msg.pop_front();
                switch(current_msg.header.id)
                {
                    
                };
            }
        }
    }

    void ClientHandler::push_back(net::Message<params::MessageType> &_msg)
    {
        mdeq_msg.push_back(_msg);
    }

    template <typename T>
    T &ClientHandler::wait_for_response(uint16_t _key, T &_var)
    {
        bool got_response = false;
 
        while(!got_response)
        {
            mdeq_msg.wait();
            if(mdeq_msg.back().header.request_id == _key || mdeq_msg.front().header.request_id == _key)
                got_response = true;
        }

        auto response = (mdeq_msg.back().header.request_id == _key) ? mdeq_msg.back() : mdeq_msg.front();
        response >> _var;
        return _var;
    }
}