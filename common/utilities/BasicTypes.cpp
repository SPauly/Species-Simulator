#include "BasicTypes.h"
#include <windows.h>
#include <stdexcept>
namespace sim
{
    namespace types
    {
        void WinConsole::create(int xpos_, int ypos_, int width_, int height_)
        {
            _hWindow = GetConsoleWindow();
            if(!GetConsoleScreenBufferInfo(_hConsole, &_consoleInfo))
                throw std::runtime_error("Could not get ConsoleScreenBufferInfo");
            
            _screen = new wchar_t[_nScreenWidth * _nScreenHeight];
            _hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
            SetConsoleActiveScreenBuffer(_hConsole);
            m_dwBytesWritten = 0;
        }
    }
}