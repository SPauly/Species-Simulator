#pragma once
#include "Params.h"
#include <windows.h>
#include <stdexcept>
#include <algorithm>
#include <memory>

namespace sim
{
    struct TSConsoleBuffer
    {
        TSConsoleBuffer();
        TSConsoleBuffer(size_t, size_t);
        ~TSConsoleBuffer();
        
        CHAR_INFO *resize(size_t);

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
        WinConsole(params::WinConsoleLayout&);
        WinConsole(size_t, size_t, size_t, size_t, size_t, size_t);
        ~WinConsole();

        bool create_console();
        bool create_console(size_t, size_t, size_t, size_t, size_t, size_t);
        bool create_console(params::WinConsoleLayout&);

        size_t write_buffer(std::shared_ptr<TSConsoleBuffer>, HANDLE *handle_ = nullptr);

        HANDLE& get_active_handle();
        params::WinConsoleLayout& get_layout();

    private:

        params::WinConsoleLayout _layout;

        SMALL_RECT _rectWindow;
        COORD coord;

        std::shared_ptr<TSConsoleBuffer>  _ptr_screen_buf = nullptr;
        HANDLE _hConsole;
        HANDLE _hConsoleOrigin;
        CONSOLE_SCREEN_BUFFER_INFO _csbi;
        CONSOLE_SCREEN_BUFFER_INFO _csbiOrigin;
    };

}