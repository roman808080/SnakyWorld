#include "Utils.h"

#include <iostream>
#include <tuple>

#include <windows.h>
#include <conio.h>


void Utils::setCursorPositon(int x, int y)
{
    std::cout.flush();
 
    // Convert to the row and column format.
    int row = y + 1;
    int column = x + 1;

    printf("\x1b[%d;%dH", row, column);
}

void Utils::showConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

Utils::Directon Utils::getCurrentDirection()
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

void Utils::clearConslole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD coordScreen = { 0, 0 };    // home for the cursor 
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer. 

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.

    if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer 
        (TCHAR) ' ',     // Character to write to the buffer
        dwConSize,       // Number of cells to write 
        coordScreen,     // Coordinates of first cell 
        &cCharsWritten))// Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Set the buffer's attributes accordingly.

    if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer 
        csbi.wAttributes, // Character attributes to use
        dwConSize,        // Number of cells to set attribute 
        coordScreen,      // Coordinates of first cell 
        &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.

    SetConsoleCursorPosition(hConsole, coordScreen);
}

std::tuple<int, int, int, int> Utils::getScreenCoordinates()
{
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    BOOL bCsbi = 0;
    bCsbi = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);
    if (!bCsbi)
    {
        return std::make_tuple(-1, -1, -1, -1);
    }

    auto bottom = bufferInfo.srWindow.Bottom;
    auto left = bufferInfo.srWindow.Left;
    auto right = bufferInfo.srWindow.Right;
    auto top = bufferInfo.srWindow.Top;

    return std::make_tuple(bufferInfo.srWindow.Bottom,
                           bufferInfo.srWindow.Left,
                           bufferInfo.srWindow.Right,
                           bufferInfo.srWindow.Top);
}
