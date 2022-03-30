#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <initializer_list>

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

        enum MapType : uint8_t
        {
            No_Walls,
            Right_Wall,
            Left_Wall,
            Top_Wall,
            Bottom_Wall,
            HAS_FILL
        };

        struct MapConfig 
        {
            uint16_t x = 0;
            uint16_t y = 0;
            uint16_t width = 0;
            uint16_t height = 0;
            
            //converted to MapType
            uint8_t WallOne = 0;
            uint8_t WallTwo = 0;
            uint8_t WallThree = 0;
            uint8_t WallFour = 0;
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