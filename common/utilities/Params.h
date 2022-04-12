#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <initializer_list>

/*  Contains all the used parameters
    Every Parameter should be able to be send via a Message object 
*/

namespace sim
{
    namespace params
    {
        //Message Header Type
        enum class MessageType : uint8_t
        {
            Client_Assigne_ID,
            Client_Register_With_Server,
            Client_Unregister_With_Server,

            Server_Get_Status,
            Server_Get_Ping,
            Server_Message,
            Server_Accept,
            Server_Denial,
            Server_Ping,

            Send_Map_Console_Layout, //WinConsoleLayout Type
            Send_Map_Layout //MapConfig Type 
        };

        // type used by Map instance
        enum MapType : uint8_t
        {
            No_Walls,
            Right_Wall,
            Left_Wall,
            Top_Wall,
            Bottom_Wall,
            HAS_FILL
        };

        // Type of Entity
        enum class EntityType : uint8_t
        {
            ENTITY,
            GAMEOBJECT,
            FOOD,
            WALL,
            AREA,
            DEATHZONE,
            FARMZONE,
            PLAYEROBJECT
        };

        // Type of Entity Style
        enum class EntityStyle : uint8_t
        {
            EMPTY = 0,
            DOT = 46,
            HASH = 35
        };

        //WinConsole Layout Messsage type
        struct WinConsoleLayout
        {
            uint16_t _nxpos = 0;
            uint16_t _nypos = 0;
            uint16_t _nScreenWidth = 60;
            uint16_t _nScreenHeight = 60;
            uint16_t _fontw = 8;
            uint16_t _fonth = 16;
        };

        //Message for sending Map Configuration
        struct MapConfig 
        {
            uint16_t x = 0;
            uint16_t y = 0;
            uint16_t width = 0;
            uint16_t height = 0;
            
            //converted to MapType
            uint16_t WallOne = 0;
            uint16_t WallTwo = 0;
            uint16_t WallThree = 0;
            uint16_t WallFour = 0;
        };
    }
}