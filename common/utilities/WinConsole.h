#pragma once
#include <windows.h>
#include <stdexcept>
#include <algorithm>

namespace sim
{
    struct buffer
    {
        buffer(int, int);
        ~buffer();

        CHAR_INFO *get_buffer();

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

        size_t write_buffer(CHAR_INFO*);
        void fill_console();

    private:
        int _nxpos = 0;
        int _nypos = 0;
        int _nScreenWidth = 60;
        int _nScreenHeight = 60;
        int _fontw = 8;
        int _fonth = 16;

        SMALL_RECT _rectWindow;
        COORD coord;

        buffer *_screen = nullptr;
        HANDLE _hConsole;
        HANDLE _hConsoleOrigin;
        CONSOLE_SCREEN_BUFFER_INFO _csbi;
        CONSOLE_SCREEN_BUFFER_INFO _csbiOrigin;
    };

}