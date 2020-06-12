#include "Console.h"

#include <iostream>
#include <tuple>

#include <windows.h>
#include <conio.h>
#include <sgr.hpp>

namespace
{
    const int kReductionRate = 2;
    const char kCeil[] = "  ";

    cpp_sgr::sgr convertColor(Console::Color color)
    {
        switch (color)
        {
        case Console::Color::Black:
            return cpp_sgr::black_bg;
        case Console::Color::Green:
            return cpp_sgr::b_green_bg;
        default:
            return cpp_sgr::black_bg;
        }
    }
}


Console::Console()
    : consoleHandler(GetStdHandle(STD_OUTPUT_HANDLE))
{
    showConsoleCursor(false);
    clear();
}

Console::~Console()
{
    CloseHandle(consoleHandler);
}

Console::Directon Console::getCurrentDirection()
{
    if (!_kbhit())
    {
        return Directon::Unknown;
    }

    switch (_getch())
    {
    case 'a':
        return Directon::Left;
    case 'd':
        return Directon::Right;
    case 'w':
        return Directon::Up;
    case 's':
        return Directon::Down;
    case 'x':
        return Directon::Stop;
    default:
        return Directon::Unknown;
    }
}

std::tuple<int, int> Console::getScreenSize()
{
    int x, y;
    std::tie(x, y) = getRealScreenSize();

    // Hides the real size of the x axe because of uneven sizes of axes.
    return std::make_tuple(x / kReductionRate, y);
}

void Console::drawCeil(Color color, int x, int y)
{
    // Fix the reduction and draw to positions.
    setRealCursorPositon(x * kReductionRate, y);
    std::cerr << convertColor(color) << kCeil;
}

void Console::clear()
{
    COORD coordScreen = { 0, 0 }; // Home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(consoleHandler, &csbi))
    {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(
        consoleHandler,  // Handle to console screen buffer
        (TCHAR) ' ',     // Character to write to the buffer
        dwConSize,       // Number of cells to write
        coordScreen,     // Coordinates of first cell
        &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(consoleHandler, &csbi))
    {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(
        consoleHandler,   // Handle to console screen buffer
        csbi.wAttributes, // Character attributes to use
        dwConSize,        // Number of cells to set attribute
        coordScreen,      // Coordinates of first cell
        &cCharsWritten))  // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(consoleHandler, coordScreen);
}

void Console::showConsoleCursor(bool showFlag)
{
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(consoleHandler, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(consoleHandler, &cursorInfo);
}

void Console::setRealCursorPositon(int x, int y)
{
    std::cout.flush();

    // Convert to the row and column format.
    int row = y + 1;
    int column = x + 1;

    // Using escape sequences to set the cursor position.
    printf("\x1b[%d;%dH", row, column);
}

std::tuple<int, int> Console::getRealScreenSize()
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    BOOL bCsbi = 0;
    bCsbi = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);
    if (!bCsbi)
    {
        return std::make_tuple(-1, -1);
    }

    return std::make_tuple(bufferInfo.srWindow.Right, bufferInfo.srWindow.Bottom);
}
