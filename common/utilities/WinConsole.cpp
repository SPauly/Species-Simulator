#include "WinConsole.h"
namespace sim
{
    buffer::buffer(int width_, int height_) : width(width_), height(height_)
    {
        char_buffer = new CHAR_INFO[width * height];
        for (int i = 0; i < width - 1; i++)
        {
            for (int ii = 0; ii < height - 1; ii++)
            {
                char_buffer[ii * width + i].Char.UnicodeChar = ' ';
                char_buffer[ii * width + i].Attributes = 0x000F;
            }
        }
    }

    buffer::buffer() : buffer::buffer(1, 1)
    {
    }

    buffer::~buffer()
    {
        delete[] char_buffer;
    }

    CHAR_INFO *buffer::get_buffer()
    {
        return char_buffer;
    }

    void buffer::write_character(int xpos_, int ypos_, const char &ch_)
    {
        if (xpos_ >= 0 && xpos_ < width && ypos_ >= 0 && ypos_ < height)
        {
            char_buffer[ypos_ * width + xpos_].Char.UnicodeChar = ch_;
            char_buffer[ypos_ * width + xpos_].Attributes = 0x000F;
        }
    }

    WinConsole::WinConsole(int xpos_, int ypos_, int width_, int height_, int fontw_, int fonth_) : _nxpos(xpos_), _nypos(ypos_), _nScreenWidth(width_), _nScreenHeight(height_), _fontw(fontw_), _fonth(fonth_)
    {
        _hConsoleOrigin = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!GetConsoleScreenBufferInfo(_hConsoleOrigin, &_csbiOrigin))
            throw std::runtime_error("Could not get ConsoleScreenBufferInfo");

        _hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!GetConsoleScreenBufferInfo(_hConsole, &_csbi))
            throw std::runtime_error("Could not get ConsoleScreenBufferInfo");
        
        create_console();
    }

    WinConsole::~WinConsole()
    {
        SetConsoleActiveScreenBuffer(_hConsoleOrigin);
        SetConsoleTextAttribute(_hConsoleOrigin, _csbiOrigin.wAttributes);
        SetConsoleScreenBufferSize(_hConsoleOrigin, _csbiOrigin.dwSize);
        SetConsoleWindowInfo(_hConsoleOrigin, TRUE, &_csbiOrigin.srWindow);
    }

    bool WinConsole::create_console(int xpos_, int ypos_, int width_, int height_, int fontw_, int fonth_)
    {
        _nxpos = xpos_;
        _nypos = ypos_;
        _nScreenHeight = height_;
        _nScreenWidth = width_;
        _fontw = fontw_;
        _fonth = fonth_;

        return this->create_console();
    }

    bool WinConsole::create_console(sim::types::ConsoleLayout& cl_)
    {
        _nxpos = cl_._nxpos;
        _nypos = cl_._nypos;
        _nScreenHeight = cl_._nScreenHeight;
        _nScreenWidth = cl_._nScreenWidth;
        _fontw = cl_._fontw;
        _fonth = cl_._fonth;

        return this->create_console();
    }

    bool WinConsole::create_console()
    {

        bool bSuccess = true;

        if (_hConsole == INVALID_HANDLE_VALUE)
            return false;

        bSuccess = GetConsoleScreenBufferInfo(_hConsole, &_csbi);

        coord = GetLargestConsoleWindowSize(_hConsole);
        // set physical window size
        _rectWindow.Left = (SHORT)_nxpos;
        _rectWindow.Top = (SHORT)_nypos;
        _rectWindow.Right = (SHORT)(std::min((SHORT)_nScreenWidth, coord.X) - 1);
        _rectWindow.Bottom = (SHORT)(std::min((SHORT)_nScreenHeight, coord.Y) - 1);

        // set console screenbuffer
        coord.X = _nScreenWidth;
        coord.Y = _nScreenHeight;

        // If the Current Buffer is Larger than what we want, Resize the
        // Console Window First, then the Buffer
        if ((DWORD)_csbi.dwSize.X * _csbi.dwSize.Y > (DWORD)_nScreenWidth * _nScreenHeight)
        {
            bSuccess = SetConsoleWindowInfo(_hConsole, TRUE, &_rectWindow);
            bSuccess = SetConsoleScreenBufferSize(_hConsole, coord);
        }

        // If the Current Buffer is Smaller than what we want, Resize the
        // Buffer First, then the Console Window
        if ((DWORD)_csbi.dwSize.X * _csbi.dwSize.Y < (DWORD)_nScreenWidth * _nScreenHeight)
        {
            bSuccess = SetConsoleScreenBufferSize(_hConsole, coord);
            bSuccess = SetConsoleWindowInfo(_hConsole, TRUE, &_rectWindow);
        }

        return bSuccess;
    }

    size_t WinConsole::write_buffer(HANDLE handle_, buffer &buf_)
    {
        if (handle_ == INVALID_HANDLE_VALUE)
            return 0;

        if (buf_.width > 0 && buf_.width <= _nScreenWidth && buf_.height > 0 && buf_.height <= _nScreenHeight)
            _screen = buf_;
        else
            return 0;

        WriteConsoleOutput(handle_, _screen.get_buffer(), {(SHORT)_screen.width, (SHORT)_nScreenHeight}, {0, 0}, &_rectWindow);
    }

    HANDLE &WinConsole::get_active_handle()
    {
        return _hConsole;
    }

}