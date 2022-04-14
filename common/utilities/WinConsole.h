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
        TSConsoleBuffer(size_t, size_t);
        ~TSConsoleBuffer();
        
        void write_character(size_t, const char&);  //write character to real position
        void write_character(size_t, size_t, const char&);    //write character to relative position of x and y

        CHAR_INFO *get_buffer();

        size_t width = 0;
        size_t height = 0;
        CHAR_INFO* char_buffer = nullptr;
    };

    class WinConsole
    {
    public:
        WinConsole() = delete;
        WinConsole(size_t, size_t, size_t, size_t, size_t, size_t);
        ~WinConsole();

        bool create_console();
        bool create_console(size_t, size_t, size_t, size_t, size_t, size_t);
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