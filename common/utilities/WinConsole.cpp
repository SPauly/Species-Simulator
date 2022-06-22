#include "WinConsole.h"

namespace sim
{
    TSConsoleBuffer::TSConsoleBuffer(size_t width_, size_t height_) : width(width_), height(height_)
    {
        char_buffer = new CHAR_INFO[width * height];
        int i;
        for(i = width * height; i--;)
        {
            char_buffer[i].Char.UnicodeChar = ' ';
            char_buffer[i].Attributes = 0x000F;
        }
    }

    TSConsoleBuffer::TSConsoleBuffer() : TSConsoleBuffer::TSConsoleBuffer(1, 1)
    {
    }

    TSConsoleBuffer::~TSConsoleBuffer()
    {
        delete[] char_buffer;
    }

    void TSConsoleBuffer::resize(size_t _size)
    {
        std::unique_lock<std::shared_mutex> lock(mux);

        if(char_buffer)
            delete[] char_buffer;
        
        char_buffer = new CHAR_INFO[_size];
        int i;
        for(i = _size; i--;)
        {
            char_buffer[i].Char.UnicodeChar = ' ';
            char_buffer[i].Attributes = 0x000F;
        }
    }

    const CHAR_INFO *TSConsoleBuffer::get_buffer()
    {  
        std::shared_lock<std::shared_mutex> lock(mux);
        return char_buffer;
    }

    void TSConsoleBuffer::write_character(size_t real_pos, const char &ch_)
    {
        std::unique_lock<std::shared_mutex> lock(mux);

        if (char_buffer && real_pos < width * height)
        {
            char_buffer[real_pos].Char.UnicodeChar = ch_;
            char_buffer[real_pos].Attributes = 0x000F;
        }
    }

    void TSConsoleBuffer::write_character(size_t xpos_, size_t ypos_, const char &ch_)
    {
        std::unique_lock<std::shared_mutex> lock(mux);

        if (char_buffer && xpos_ >= 0 && xpos_ < width && ypos_ >= 0 && ypos_ < height)
        {
            char_buffer[ypos_ * width + xpos_].Char.UnicodeChar = ch_;
            char_buffer[ypos_ * width + xpos_].Attributes = 0x000F;
        }
    }

    void TSConsoleBuffer::write_buffer_to_console(WinConsole* con_ptr_, HANDLE handle_)
    {
        std::shared_lock<std::shared_mutex> lock(mux);

        if(!con_ptr_)
            return;
        
        con_ptr_->write_raw_buffer(char_buffer, width, height, handle_);
        return;
    }

    WinConsole::WinConsole(params::WinConsoleLayout &layout)
    {
        _hConsoleOrigin = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!GetConsoleScreenBufferInfo(_hConsoleOrigin, &_csbiOrigin))
            throw std::runtime_error("Could not get ConsoleScreenBufferInfo");

        _hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!GetConsoleScreenBufferInfo(_hConsole, &_csbi))
            throw std::runtime_error("Could not get ConsoleScreenBufferInfo");

        create_console(layout);
    }

    WinConsole::WinConsole(size_t xpos_, size_t ypos_, size_t width_, size_t height_, size_t fontw_, size_t fonth_)
    {
        _hConsoleOrigin = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!GetConsoleScreenBufferInfo(_hConsoleOrigin, &_csbiOrigin))
            throw std::runtime_error("Could not get ConsoleScreenBufferInfo");

        _hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!GetConsoleScreenBufferInfo(_hConsole, &_csbi))
            throw std::runtime_error("Could not get ConsoleScreenBufferInfo");

        create_console(xpos_, ypos_, width_, height_, fontw_, fonth_);
    }

    WinConsole::~WinConsole()
    {
        SetConsoleActiveScreenBuffer(_hConsoleOrigin);
        SetConsoleTextAttribute(_hConsoleOrigin, _csbiOrigin.wAttributes);
        SetConsoleScreenBufferSize(_hConsoleOrigin, _csbiOrigin.dwSize);
        SetConsoleWindowInfo(_hConsoleOrigin, TRUE, &_csbiOrigin.srWindow);
    }

    bool WinConsole::create_console(size_t xpos_, size_t ypos_, size_t width_, size_t height_, size_t fontw_, size_t fonth_)
    {
        _layout._nxpos = xpos_;
        _layout._nypos = ypos_;
        _layout._nScreenHeight = height_;
        _layout._nScreenWidth = width_;
        _layout._fontw = fontw_;
        _layout._fonth = fonth_;

        return this->create_console();
    }

    bool WinConsole::create_console(params::WinConsoleLayout &cl_)
    {
        _layout = std::move(cl_);

        return this->create_console();
    }

    bool WinConsole::create_console()
    {

        bool bSuccess = true;

        if (_hConsole == INVALID_HANDLE_VALUE)
            return false;

        // get screenbuffeer info
        bSuccess = GetConsoleScreenBufferInfo(_hConsole, &_csbi);

        // set physical window size
        coord = GetLargestConsoleWindowSize(_hConsole);
        _rectWindow.Left = (SHORT)_layout._nxpos;
        _rectWindow.Top = (SHORT)_layout._nypos;
        _rectWindow.Right = (SHORT)(std::min((SHORT)_layout._nScreenWidth, coord.X) - 1);
        _rectWindow.Bottom = (SHORT)(std::min((SHORT)_layout._nScreenHeight, coord.Y) - 1);

        // set window to minimal size
        SMALL_RECT const minimal_window = {0, 0, 1, 1};
        bSuccess = SetConsoleWindowInfo(_hConsole, TRUE, &minimal_window);

        // set console screenbuffer
        coord.X = _layout._nScreenWidth;
        coord.Y = _layout._nScreenHeight;

        // If the Current Buffer is Larger than what we want, Resize the
        // Console Window First, then the Buffer
        if ((DWORD)_csbi.dwSize.X * _csbi.dwSize.Y > (DWORD)_layout._nScreenWidth * _layout._nScreenHeight)
        {
            bSuccess = SetConsoleWindowInfo(_hConsole, TRUE, &_rectWindow);
            bSuccess = SetConsoleScreenBufferSize(_hConsole, coord);
        }

        // If the Current Buffer is Smaller than what we want, Resize the
        // Buffer First, then the Console Window
        if ((DWORD)_csbi.dwSize.X * _csbi.dwSize.Y < (DWORD)_layout._nScreenWidth * _layout._nScreenHeight)
        {
            bSuccess = SetConsoleScreenBufferSize(_hConsole, coord);
            bSuccess = SetConsoleWindowInfo(_hConsole, TRUE, &_rectWindow);
        }

        if (!SetConsoleActiveScreenBuffer(_hConsole))
            return bSuccess = false;

        return bSuccess;
    }

    size_t WinConsole::write_raw_buffer(CHAR_INFO *buf_, size_t width_, size_t height_, HANDLE handle_)
    {
        if (handle_ == INVALID_HANDLE_VALUE || !handle_)
        {
            handle_ = _hConsole;
        }

        WriteConsoleOutput(handle_, buf_, {(SHORT)width_, (SHORT)height_}, {0, 0}, &_rectWindow);
    }

    HANDLE &WinConsole::get_active_handle()
    {
        return _hConsole;
    }

    params::WinConsoleLayout &WinConsole::get_layout()
    {
        return _layout;
    }

}