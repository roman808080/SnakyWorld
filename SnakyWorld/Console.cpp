#include "Console.h"

#include <iostream>
#include <tuple>

#include <windows.h>
#include <conio.h>
#include <sgr.hpp>

namespace
{
    const int kReductionRate = 2;

    const int kArrowKey = 224;
    const int kArrowUp = 72;
    const int kArrowLeft = 75;
    const int kArrowRight = 77;
    const int kArrowDown = 80;

    const char kCell[] = "  ";

    cpp_sgr::sgr convertColor(Console::Color color)
    {
        switch (color)
        {
        case Console::Color::Black:
            return cpp_sgr::black_bg;
        case Console::Color::Green:
            return cpp_sgr::b_green_bg;
        case Console::Color::Red:
            return cpp_sgr::b_red_bg;
        case Console::Color::Yellow:
            return cpp_sgr::yellow_bg;
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

    auto symbol = _getch();
    switch (symbol)
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
    case kArrowKey:
        return getArrowDirection();
    default:
        return Directon::Unknown;
    }
}

std::tuple<int, int> Console::getScreenSize()
{
    int x, y;
    std::tie(x, y) = getRealScreenSize();

    // Hides the real size of the y axe because of uneven sizes of axes.
    return std::make_tuple(x, y / kReductionRate);
}

void Console::drawCell(Color color, int x, int y)
{
    // Fix the reduction and draw to positions.
    setRealCursorPositon(x, y * kReductionRate);
    std::cerr << convertColor(color) << kCell;
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

void Console::setRealCursorPositon(int row, int column)
{
    std::cout.flush();

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

    return std::make_tuple(bufferInfo.srWindow.Bottom, bufferInfo.srWindow.Right);
}

Console::Directon Console::getArrowDirection()
{
    if (!_kbhit())
    {
        return Directon::Unknown;
    }

    switch (_getch())
    {
    case kArrowLeft:
        return Directon::Left;
    case kArrowRight:
        return Directon::Right;
    case kArrowUp:
        return Directon::Up;
    case kArrowDown:
        return Directon::Down;
    case kArrowKey:
        return getArrowDirection();
    default:
        return Directon::Unknown;
    }
}
