#include "BasicTypes.h"
#include <windows.h>
#include <algorithm>
#include <stdexcept>
namespace sim
{
    namespace types
    {

        WinConsole::WinConsole(int xpos_, int ypos_, int _width, int height_) : _nxpos(xpos_), _nypos(ypos_), _nScreenWidth(width_), _nScreenHeight(height_)
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
            delete[] _screen;
        }

        bool WinConsole::create_console(int xpos_, int ypos_, int width_, int height_)
        {
            _nxpos = xpos_;
            _nypos = ypos_;
            _nScreenHeight = height_;
            _nScreenWidth = width_;

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

            // create active buffer for us
            _screen = new wchar_t[_nScreenWidth * _nScreenHeight];
            memset(_screen, '.', sizeof(wchar_t) * _nScreenWidth * _nScreenHeight);

            return bSuccess;
        }
    }
}