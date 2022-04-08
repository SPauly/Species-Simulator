#pragma once
#include "NetInclude.h"
#include "BasicTypes.h"
#include "Params.h"
#include "WinConsole.h"
#include "Map.h"

namespace sim
{
    class Client : public net::Client_Interface<params::MessageType>
    {
    public:
        void run();
    private:
        std::shared_ptr<sim::WinConsole> m_console;
        sim::params::WinConsoleLayout cl;
    };
}