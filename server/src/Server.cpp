#include "Server.h"

namespace sim
{
    Server::Server(uint16_t nport_, size_t nmapSize_) : sim::net::Server_Interface<sim::params::MessageType>(nport_),
                                                        m_nMapsCount(nmapSize_)
    {
    }

    void Server::run(size_t nMaxMesseges = -1, bool bWait = false)
    {
        if (!this->start_server())
        {
            // inform of failure
            return;
        }

        if (!this->mf_start_work_thread())
        {
            // inform of failure
            return;
        }

        std::cout << "Specify Window size x: ";
        std::cin>>x;
        std::cout <<"Specify window height y: ";
        std::cin >> y;
        m_console.create_console(0,0,x,y,8,16);

        while (true)
        {
            this->update(nMaxMesseges, bWait);
            this->mf_update_work();
        }
    }

    void Server::on_client_validated(std::shared_ptr<net::Connection<params::MessageType>> client)
    {
        test_console();
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

    void Server::test_console()
    {
        sim::buffer m_buffer{x,y};
        sim::types::rand random;

        for(int i = 0; i < x; i++)
        {
            for(int ii = 0; ii < y; ii++)
            {
                if(random.get_rand(0,100) > 50)
                {
                    m_buffer.write_character(i,ii, '#');
                }
            }
        }

        m_console.write_buffer(m_console.get_active_handle(), m_buffer);
    }

    bool Server::mf_start_work_thread()
    {

        // start work in thread
        try
        {
            //m_WorkThread = std::thread([this]()
            //                           { mf_update_work(); });
        }
        catch (const std::exception &e)
        {
            // inform of failure
            return false;
        }

        return true;
    }
    void Server::mf_update_work()
    {
    }
}