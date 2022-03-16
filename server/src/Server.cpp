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

        while (true)
        {
            this->update(nMaxMesseges, bWait);
            this->mf_update_work();
        }
    }

    void Server::on_client_validated(std::shared_ptr<net::Connection<params::MessageType>> client)
    {
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

    bool Server::mf_start_work_thread()
    {
        // set up window    
        m_hWindow = GetConsoleWindow(); 
        if(m_hWindow != NULL) {
            SetWindowPos(m_hWindow ,0,0,0 ,1000,300,SWP_SHOWWINDOW|SWP_NOMOVE);
        }
        m_screen = new wchar_t[m_nScreenWidth * m_nScreenHeight];
        m_hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(m_hConsole);
        m_dwBytesWritten = 0;

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