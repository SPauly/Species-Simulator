#include "BasicTypes.h"
#include <windows.h>
namespace sim
{
    namespace types
    {
        void WinConsole::create(int xpos_, int ypos_, int width_, int height_)
        {
            _hWindow = GetConsoleWindow();
            if(!GetConsoleScreenBufferInfo(_hConsole, &))
            m_screen = new wchar_t[m_nScreenWidth * m_nScreenHeight];
            m_hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
            SetConsoleActiveScreenBuffer(m_hConsole);
            m_dwBytesWritten = 0;
        }
    }
}