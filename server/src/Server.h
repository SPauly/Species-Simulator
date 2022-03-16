#pragma once
#include <cstdint>
#include <thread>
#include "NetInclude.h"
#include "Params.h"
#include "Environment.h"

namespace sim
{
    class Server : public net::Server_Interface<params::MessageType>
    {
    public:
        Server(uint16_t, size_t);

        void run(size_t, bool); //arg: Max Number of Messages processed, bwait, number of Maps/Clients calls start_server, update Server and runs server
        
        virtual void on_client_validated(std::shared_ptr<net::Connection<params::MessageType>>) override;

    private:    
        bool mf_start_work_thread();
        void mf_update_work();

    protected:
        virtual bool on_client_connect(std::shared_ptr<sim::net::Connection<sim::params::MessageType>>) override;
        virtual void on_client_disconnect(std::shared_ptr<sim::net::Connection<sim::params::MessageType>>) override;
        virtual void on_message(std::shared_ptr<sim::net::Connection<sim::params::MessageType>>, sim::net::Message<sim::params::MessageType> &) override;

    private:
        //window dependencies
        int m_nScreenWidth = 130;
        int m_nScreenHeight = 100;
        wchar_t* m_screen = nullptr;
        HANDLE m_hConsole;
        HWND m_hWindow;
        DWORD m_dwBytesWritten = 0;

        std::thread m_WorkThread;

        size_t m_nMapsCount = 0;

        sim::Environment m_environment{};
    };

}