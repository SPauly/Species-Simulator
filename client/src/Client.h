#pragma once
#include "../common/network/NetInclude.h"
#include "../common/utilities/BasicTypes.h"

namespace sim
{
    class Client : public net::Client_Interface<params::MessageType>
    {

    };
}