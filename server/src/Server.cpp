#include "Server.h"
#include <chrono>

namespace sim
{
    Server::Server(uint16_t nport_, size_t nmapSize_) : net::Server_Interface<params::MessageType>(nport_),
                                                        m_nMapsCount(nmapSize_)
    {
        mptr_buffer = std::make_shared<TSConsoleBuffer>(x, y);
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

        if (m_asio_update_thread.joinable())
            m_asio_update_thread.join();
    }

    void Server::on_client_validated(std::shared_ptr<net::Connection<params::MessageType>> client)
    {
        mvec_client_handlers.push_back(std::make_shared<ClientHandler>(client));
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
        switch(msg.header.id)
        {
            case params::MessageType::Send_Screen_Buffer:
                //forward to screen buffer
                break;
            case params::MessageType::Direct_Response:
                message_client(msg.header.receiver, msg);
                break;
            case params::MessageType::Response:
                (msg.header.receiver != 0) ? mvec_client_handlers.at(msg.header.receiver - 10000)->push_back(msg)
                    : mvec_client_handlers.at(client->get_uid() - 10000)->push_back(msg); 
                break;
            case params::MessageType::Request:
                (msg.header.receiver != 0) ? mvec_client_handlers.at(msg.header.receiver - 10000)->push_back(msg)
                    : mvec_client_handlers.at(client->get_uid() - 10000)->push_back(msg); 
                break;
            default:
                mvec_client_handlers.at(client->get_uid() - 10000)->push_back(msg);
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