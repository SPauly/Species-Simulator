#pragma once
#include "NetInclude.h"
#include "BasicTypes.h"
#include "Params.h"
#include "WinConsole.h"

namespace sim
{
    class Client : public net::Client_Interface<params::MessageType>
    {
    public:
        void run();
    private:
        sim::WinConsole m_console{0,0,60,30,8,16};
        sim::types::ConsoleLayout cl;
    };
}