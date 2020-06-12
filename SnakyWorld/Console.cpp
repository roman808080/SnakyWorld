#include "Console.h"

#include <iostream>
#include <tuple>

#include <windows.h>
#include <conio.h>
#include <sgr.hpp>

namespace
{
    const int kReductionRate = 2;
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
        return Directon::UNDEFINED;
    }

    switch (_getch())
    {
    case 'a':
        return Directon::LEFT;
    case 'd':
        return Directon::RIGHT;
    case 'w':
        return Directon::UP;
    case 's':
        return Directon::DOWN;
    case 'x':
        return Directon::STOP;
    default:
        return Directon::UNDEFINED;
    }
}

std::tuple<int, int> Console::getScreenSize()
{
    int x, y;
    std::tie(x, y) = getRealScreenSize();
    return std::make_tuple(x / kReductionRate, y);
}

void Console::drawCeil(int x, int y)
{
    setRealCursorPositon(x * kReductionRate, y);
    std::cerr << cpp_sgr::b_green_bg << "  ";
}

void Console::clear()
{
    COORD coordScreen = { 0, 0 }; // home for the cursor 
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
