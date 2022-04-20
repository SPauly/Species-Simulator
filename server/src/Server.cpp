#include "Server.h"
#include <chrono>

namespace sim
{
    Server::Server(uint16_t nport_, size_t nmapSize_) : net::Server_Interface<params::MessageType>(nport_),
                                                        m_nMapsCount(nmapSize_)
    {
        // set screen with to full hd with font size 8x16 pixels
        x = 120;
        y = 30;
        m_console = std::make_shared<WinConsole>(0, 0, x, y, 8, 16);

        // initialize vectors needed for transfer of Entities
        m_incomming_entities.resize(m_nMapsCount, std::vector<Entity>());

        // create environment with the specific coordinates
        m_envConfig.width = x;
        m_envConfig.height = 25;
        m_environment = std::make_unique<Environment>(m_console, m_envConfig, m_nMapsCount, m_incomming_entities);
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
        net::Message<params::MessageType> msg;
        msg.header.id = params::MessageType::Send_Map_Console_Layout;
        msg << m_environment->at_get_map(this->get_connections() - 1).get_layout();
        client->send(msg);

        // send Map
        net::Message<params::MessageType> map_msg;
        map_msg.header.id = params::MessageType::Send_Map_Layout;
        map_msg << m_environment->at_get_map(this->get_connections() - 1).get_config();
        client->send(map_msg);

        // send entities
        net::Message<params::MessageType> ent_msg;
        ent_msg.header.id = params::MessageType::Send_Entities_Size;
        ent_msg << m_environment->at_get_map(this->get_connections() - 1).get_entities_size();
        client->send(ent_msg);
        ent_msg.header.id = params::MessageType::Send_Entities;
        ent_msg.push_back_complex<Entity>(ent_msg, m_incomming_entities.at(this->get_connections() - 1).data(), m_incomming_entities.at(this->get_connections() - 1).size());
        client->send(ent_msg);
    }

    bool Server::on_client_connect(std::shared_ptr<net::Connection<params::MessageType>> client)
    {
        if (this->get_connections() < m_nMapsCount)
        {
            net::Message<params::MessageType> msg;
            msg.header.id = params::MessageType::Server_Accept;
            client->send(msg);
            return true;
        }
        else
        {
            net::Message<params::MessageType> msg;
            msg.header.id = params::MessageType::Server_Denial;
            msg << "Max Clients connected";
            client->send(msg);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return false;
        }
    }

    void Server::on_client_disconnect(std::shared_ptr<net::Connection<params::MessageType>> client)
    {
        std::cout << "[" << client->get_uid() << "] was removed. \n";
    }

    void Server::on_message(std::shared_ptr<net::Connection<params::MessageType>> client, net::Message<params::MessageType> &msg)
    {
        switch (msg.header.id)
        {
        }
    }

    void Server::test_console()
    {
        TSConsoleBuffer m_buffer{x, y};

        for (int i = 0; i < x; i++)
        {
            for (int ii = 0; ii < y; ii++)
            {
                if (51 > 50)
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