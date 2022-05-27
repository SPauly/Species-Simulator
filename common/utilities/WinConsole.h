#pragma once
#include "Params.h"
#include <windows.h>
#include <stdexcept>
#include <shared_mutex>

namespace sim
{
    class WinConsole;
    
    class TSConsoleBuffer
    {
    public:
        TSConsoleBuffer();
        TSConsoleBuffer(size_t, size_t);
        virtual ~TSConsoleBuffer();

        TSConsoleBuffer(const TSConsoleBuffer&) = delete;
        
        void resize(size_t);

        void write_character(size_t, const char&);  //write character to real position
        void write_character(size_t, size_t, const char&);    //write character to relative position of x and y
        void write_buffer_to_console(WinConsole*, HANDLE *handle_ = nullptr);

        const CHAR_INFO *get_buffer();
        const size_t &get_width();
        const size_t &get_height();

    private:
        mutable std::shared_mutex mux;
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

        size_t write_raw_buffer(CHAR_INFO *, size_t, size_t, HANDLE *handle_ = nullptr);

        HANDLE& get_active_handle();
        params::WinConsoleLayout& get_layout();

    private:
        params::WinConsoleLayout _layout;

        SMALL_RECT _rectWindow;
        COORD coord;

        HANDLE _hConsole;
        HANDLE _hConsoleOrigin;
        CONSOLE_SCREEN_BUFFER_INFO _csbi;
        CONSOLE_SCREEN_BUFFER_INFO _csbiOrigin;
    };

}