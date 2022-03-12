#pragma once
#include <cstdint>
#include <string>
#include "../network/NetInclude.h"

namespace sim
{
    namespace params
    {

        enum class MessageType : uint8_t
        {
            Client_Accepted,
            Client_Assigne_ID,
            Client_Register_With_Server,
            Client_Unregister_With_Server,

            Server_Get_Status,
            Server_Get_Ping,
            Server_Message,
            Server_Accept,
            Server_Ping
        };

        enum class MapType : uint8_t
        {
            Standalone,
            Right_Open,
            Left_Open,
            Top_Open,
            Bottom_Open,
            Environment
        };
        
    	enum class EntityType : uint8_t
        {
            Player,
            Wall
        };

        enum class EntityStyle : uint8_t
        {
            DOT = 46,
            HASH = 35
        };
    }
}