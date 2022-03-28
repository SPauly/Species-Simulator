#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

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
            Server_Ping,

            Send_Console_Layout,
            Send_Map_Layout
        };

        enum class MapType : uint8_t
        {
            Standalone,
            Right_Open,
            Left_Open,
            Top_Open,
            Bottom_Open,
            HAS_FILL
        };

        struct MapConfig 
        {
            uint16_t width = 0;
            uint16_t height = 0;
        };
        
    	enum class EntityType : uint8_t
        {
            Player,
            Object
        };

        enum class EntityStyle : uint8_t
        {
            DOT = 46,
            HASH = 35
        };

        struct param
        {
            union{
                uint8_t small_size;
                uint32_t mittle_size;
                uint64_t size;
                char symbol;
                } type_param;
            std::string name;
            std::function<bool(int)> &lambda;
        };
 
        class config
        {
        };
    }
}