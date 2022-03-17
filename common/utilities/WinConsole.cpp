#include "WinConsole.h"
namespace sim
{
    buffer::buffer(int width, int height){
        char_buffer = new CHAR_INFO[width*height];
        for( int i = 0; i < width - 1; i++)
        {
            for(int ii = 0; ii < height - 1; ii++)
            {
                char_buffer[ii * width + i].Char.UnicodeChar = '.';
                char_buffer[ii * width + i].Attributes = 0x000F;
            }
        }
    }

    buffer::~buffer()
    {
        delete[] char_buffer;
    }

    CHAR_INFO* buffer::get_buffer()
    {
        return char_buffer;
    }


    WinConsole::WinConsole(int xpos_, int ypos_, int width_, int height_, int fontw_, int fonth_) : _nxpos(xpos_), _nypos(ypos_), _nScreenWidth(width_), _nScreenHeight(height_), _fontw(fontw_), _fonth(fonth_)
    {
        _hConsoleOrigin = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!GetConsoleScreenBufferInfo(_hConsoleOrigin, &_csbiOrigin))
            throw std::runtime_error("Could not get ConsoleScreenBufferInfo");

        _hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!GetConsoleScreenBufferInfo(_hConsole, &_csbi))
            throw std::runtime_error("Could not get ConsoleScreenBufferInfo");

        if(create_console())
        {
            _screen = new buffer{_nScreenWidth,_nScreenHeight};
            fill_console();
        }


    }

    WinConsole::~WinConsole()
    {
        SetConsoleActiveScreenBuffer(_hConsoleOrigin);
        SetConsoleTextAttribute(_hConsoleOrigin, _csbiOrigin.wAttributes);
        SetConsoleScreenBufferSize(_hConsoleOrigin, _csbiOrigin.dwSize);
        SetConsoleWindowInfo(_hConsoleOrigin, TRUE, &_csbiOrigin.srWindow);
        delete _screen;
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

    bool WinConsole::create_console()
    {
        bool bSuccess = true;

        if (_hConsole == INVALID_HANDLE_VALUE)
            return false;

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

    void WinConsole::fill_console()
    {
        //_screen[_nScreenWidth * _nScreenHeight - 1] = '\0';
        WriteConsoleOutput(_hConsole, _screen->get_buffer(), {(SHORT)_nScreenWidth, (SHORT)_nScreenHeight}, {0, 0}, &_rectWindow);
    }

}