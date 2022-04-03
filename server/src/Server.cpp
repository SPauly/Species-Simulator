#include "Server.h"

namespace sim
{
    Server::Server(uint16_t nport_, size_t nmapSize_) : sim::net::Server_Interface<sim::params::MessageType>(nport_),
                                                        m_nMapsCount(nmapSize_)
    {
        m_console = std::make_shared<sim::WinConsole>(0,0,100,50,8,16);
        std::cout << "Specify Window size x: ";
        std::cin >> x;
        std::cout << "Specify window height y: ";
        std::cin >> y;
        
        m_console->create_console(0,0,x,y,8,16);
        m_mapConfig.width = x;
        m_mapConfig.height = y;
        m_environment = std::make_unique<sim::Environment>(m_console, m_mapConfig, m_nMapsCount);
        m_environment->instanciate_maps();
    }

    void Server::mf_get_config()
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

        while (true)
        {
            this->update(nMaxMesseges, bWait);
        }

        if (m_WorkThread.joinable())
            m_WorkThread.join();
    }

    void Server::on_client_validated(std::shared_ptr<net::Connection<params::MessageType>> client)
    {
        // send Window
        sim::net::Message<sim::params::MessageType> msg;
        msg.header.id = sim::params::MessageType::Send_Console_Layout;
        sim::params::WinConsoleLayout cl;
        cl._nScreenHeight = y / m_nMapsCount;
        cl._nScreenWidth = x / m_nMapsCount;
        msg << cl;
        client->send(msg);

        // send Map
        sim::net::Message<sim::params::MessageType> map_msg;
        map_msg.header.id = sim::params::MessageType::Send_Map_Layout;
        map_msg << m_environment->at_get_config(this->get_connections() - 1);
        client->send(map_msg);
    }

    bool Server::on_client_connect(std::shared_ptr<sim::net::Connection<sim::params::MessageType>> client)
    {
        if (this->get_connections() + 1 < m_nMapsCount)
        {
            sim::net::Message<sim::params::MessageType> msg;
            msg.header.id = sim::params::MessageType::Server_Accept;
            client->send(msg);
            return true;
        }
        else
        {
            sim::net::Message<sim::params::MessageType> msg;
            msg.header.id = sim::params::MessageType::Server_Denial;
            msg << "Max Clients connected";
            client->send(msg);
            return false;
        }
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
        sim::TSConsoleBuffer m_buffer{x, y};
        sim::types::rand random;

        for (int i = 0; i < x; i++)
        {
            for (int ii = 0; ii < y; ii++)
            {
                if (random.get_rand(0, 100) > 50)
                {
                    m_buffer.write_character(i, ii, '#');
                }
            }
        }

        m_console->write_buffer(m_console->get_active_handle(), m_buffer);
    }

    bool Server::mf_start_work_thread()
    {

        // start work in thread
        try
        {
            m_WorkThread = std::thread([this]()
                                       { mf_update_work(); });
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