#include "Server.h"
#include <chrono>

namespace sim
{
    Server::Server(uint16_t nport_, size_t nmapSize_) : net::Server_Interface<params::MessageType>(nport_),
                                                        m_nMapsCount(nmapSize_)
    {
        // initialize vectors needed for transfer of Entities
        m_entities.resize(m_nMapsCount, TSVector<Entity>());
        m_change_buffer.reserve(m_nMapsCount);
        // create environment with the specific coordinates
        m_envConfig.width = x;
        m_envConfig.height = 30;
        mptr_buffer = std::make_shared<TSConsoleBuffer>(x, y);
        m_environment = std::make_unique<Environment>(m_console, m_envConfig, mptr_buffer, m_nMapsCount, &m_entities, &m_change_buffer);
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

        try
        {
            m_EnvThread = std::thread([this]()
                                      { m_environment->run(); });
        }
        catch (const std::exception &e)
        {
            // inform of failure
            return;
        }

        m_asio_update_thread = std::thread([this, nMaxMesseges, bWait]()
                                           {
        while (true)
        {
            this->update(nMaxMesseges, bWait);
        } });

        // rendering loop
        while (true)
        {
            mptr_buffer->write_buffer_to_console(&m_console);
        }

        if (m_EnvThread.joinable())
            m_EnvThread.join();
        if (m_asio_update_thread.joinable())
            m_asio_update_thread.join();
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
        ent_msg.clear();
        ent_msg.header.id = params::MessageType::Send_Entities;
        ent_msg.push_back_complex<Entity>(ent_msg, m_entities.at(this->get_connections() - 1).get_vector().data(), m_entities.at(this->get_connections() - 1).size());
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
        case params::MessageType::Send_Entities:
            std::shared_ptr<std::vector<Entity>> temp_buf = std::make_shared<std::vector<Entity>>((msg.size()*8)/sizeof(Entity));
            msg.pull_complex<Entity>(msg, temp_buf->data(), (msg.size()*8)/sizeof(Entity));
            m_change_buffer.at(client->get_uid() - 10000).push_back(temp_buf);
        break;
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

        m_buffer.write_buffer_to_console(&m_console);
    }
}