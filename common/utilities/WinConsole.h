#pragma once
#include "Params.h"
#include <windows.h>
#include <stdexcept>
#include <algorithm>

namespace sim
{
    struct TSConsoleBuffer
    {
        TSConsoleBuffer();
        TSConsoleBuffer(int, int);
        ~TSConsoleBuffer();
        
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
        bool create_console(params::WinConsoleLayout&);

        size_t write_buffer(HANDLE, TSConsoleBuffer&); /*prefered function since the overload is untested so far */
        size_t write_buffer(HANDLE, TSConsoleBuffer&, params::WinConsoleLayout&);

        HANDLE& get_active_handle();
        params::WinConsoleLayout& get_layout();

    private:

        params::WinConsoleLayout _layout;

        SMALL_RECT _rectWindow;
        COORD coord;

        TSConsoleBuffer  _screen;
        HANDLE _hConsole;
        HANDLE _hConsoleOrigin;
        CONSOLE_SCREEN_BUFFER_INFO _csbi;
        CONSOLE_SCREEN_BUFFER_INFO _csbiOrigin;
    };

}