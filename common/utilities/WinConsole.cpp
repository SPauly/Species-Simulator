#include "WinConsole.h"
namespace sim
{
    TSConsoleBuffer::TSConsoleBuffer(int width_, int height_) : width(width_), height(height_)
    {
        char_buffer = new CHAR_INFO[width * height];
        for (int i = 0; i < width; i++)
        {
            for (int ii = 0; ii < height; ii++)
            {
                char_buffer[ii * width + i].Char.UnicodeChar = ' ';
                char_buffer[ii * width + i].Attributes = 0x000F;
            }
        }
    }

    TSConsoleBuffer::TSConsoleBuffer() : TSConsoleBuffer::TSConsoleBuffer(1, 1)
    {
    }

    TSConsoleBuffer::~TSConsoleBuffer()
    {
        delete[] char_buffer;
    }

    CHAR_INFO *TSConsoleBuffer::get_buffer()
    {
        return char_buffer;
    }

    void TSConsoleBuffer::write_character(int xpos_, int ypos_, const char &ch_)
    {
        if (xpos_ >= 0 && xpos_ < width && ypos_ >= 0 && ypos_ < height)
        {
            char_buffer[ypos_ * width + xpos_].Char.UnicodeChar = ch_;
            char_buffer[ypos_ * width + xpos_].Attributes = 0x000F;
        }
    }

    WinConsole::WinConsole(int xpos_, int ypos_, int width_, int height_, int fontw_, int fonth_)
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

    bool WinConsole::create_console(int xpos_, int ypos_, int width_, int height_, int fontw_, int fonth_)
    {
        _layout._nxpos = xpos_;
        _layout._nypos = ypos_;
        _layout._nScreenHeight = height_;
        _layout._nScreenWidth = width_;
        _layout._fontw = fontw_;
        _layout._fonth = fonth_;

        return this->create_console();
    }

    bool WinConsole::create_console(sim::params::WinConsoleLayout& cl_)
    {
        _layout = std::move(cl_);

        return this->create_console();
    }

    bool WinConsole::create_console()
    {

        bool bSuccess = true;

        if (_hConsole == INVALID_HANDLE_VALUE)
            return false;

        // set physical window size
        coord = GetLargestConsoleWindowSize(_hConsole);
        _rectWindow.Left = (SHORT)_layout._nxpos;
        _rectWindow.Top = (SHORT)_layout._nypos;
        _rectWindow.Right = (SHORT)(std::min((SHORT)_layout._nScreenWidth, coord.X) - 1);
        _rectWindow.Bottom = (SHORT)(std::min((SHORT)_layout._nScreenHeight, coord.Y) - 1);

        //set window to minimal size
        SMALL_RECT const minimal_window = {0,0,1,1};    
        bSuccess = SetConsoleWindowInfo(_hConsole, TRUE, &minimal_window);

        //get screenbuffeer info
        bSuccess = GetConsoleScreenBufferInfo(_hConsole, &_csbi);

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

        if(!SetConsoleActiveScreenBuffer(_hConsole))
            return bSuccess = false;

        return bSuccess;
    }

    size_t WinConsole::write_buffer(HANDLE handle_, TSConsoleBuffer &buf_)
    {
        if (handle_ == INVALID_HANDLE_VALUE)
            return 0;

        if (buf_.width > 0 && buf_.width <= _layout._nScreenWidth && buf_.height > 0 && buf_.height <= _layout._nScreenHeight)
            _screen = buf_;
        else
            return 0;

        WriteConsoleOutput(handle_, _screen.get_buffer(), {(SHORT)_layout._nScreenWidth, (SHORT)_layout._nScreenHeight}, {0, 0}, &_rectWindow);
    }

    size_t WinConsole::write_buffer(HANDLE handle_, TSConsoleBuffer &buf_, sim::params::WinConsoleLayout &conLay_)
    {
        if (handle_ == INVALID_HANDLE_VALUE)
            return 0;

        if (buf_.width > 0 && buf_.width <= conLay_._nScreenWidth && buf_.height > 0 && buf_.height <= conLay_._nScreenHeight)
            _screen = buf_;
        else
            return 0;

        WriteConsoleOutput(handle_, _screen.get_buffer(), {(SHORT)conLay_._nScreenWidth, (SHORT)conLay_._nScreenHeight}, {0, 0}, &_rectWindow);
    }

    HANDLE &WinConsole::get_active_handle()
    {
        return _hConsole;
    }

    sim::params::WinConsoleLayout &WinConsole::get_layout()
    {
        return _layout;
    }

}