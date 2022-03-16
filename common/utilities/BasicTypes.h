#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Params.h"

namespace sim
{
    namespace types
    {
        template <typename T, typename S>
        struct Entity
        {
            uint16_t x_pos = 0;
            uint16_t y_pos = 0;

            T type = 0;
            S style = 0;
        };

        template <typename T>
        class Map
        {
        public:
            Map(T);
            virtual ~Map();

            void add_entity();

        private:
            uint16_t m_width = 0;
            uint16_t m_height = 0;

            std::wstring m_layout = 0;

            T m_config{};

            std::vector<Entity<sim::params::EntityType, sim::params::EntityStyle>> m_vecEntities;
        };

        struct WinConsole
        {
            WinConsole() = default;
            WinConsole(int, int, int, int);
            ~WinConsole();

            void create();
            void create(int, int, int, int);


            int _nScreenWidth = 130;
            int _nScreenHeight = 100;

            wchar_t *_screen = nullptr;
            HANDLE _hConsole;
            HWND _hWindow;
            CONSOLE_SCREEN_BUFFER_INFO _consoleInfo;
            DWORD m_dwBytesWritten = 0;
        };
    }
}