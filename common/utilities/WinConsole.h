#pragma once
#include "BasicTypes.h"
#include <windows.h>
#include <stdexcept>
#include <algorithm>

namespace sim
{
    struct buffer
    {
        buffer();
        buffer(int, int);
        ~buffer();
        
        void write_character(int, int, const char&);

        CHAR_INFO *get_buffer();

        int width = 0;
        int height = 0;
        CHAR_INFO* char_buffer = nullptr;
    };

    class WinConsole
    {
    public:
        WinConsole() = delete;
        WinConsole(int, int, int, int, int, int);
        ~WinConsole();

        bool create_console();
        bool create_console(int, int, int, int, int, int);
        bool create_console(sim::types::ConsoleLayout&);

        size_t write_buffer(HANDLE, buffer&);

        HANDLE& get_active_handle();

    private:
        int _nxpos = 0;
        int _nypos = 0;
        int _nScreenWidth = 60;
        int _nScreenHeight = 60;
        int _fontw = 8;
        int _fonth = 16;

        SMALL_RECT _rectWindow;
        COORD coord;

        buffer  _screen;
        HANDLE _hConsole;
        HANDLE _hConsoleOrigin;
        CONSOLE_SCREEN_BUFFER_INFO _csbi;
        CONSOLE_SCREEN_BUFFER_INFO _csbiOrigin;
    };

}