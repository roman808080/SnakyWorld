#include "Console.h"

#include <iostream>
#include <iomanip>
#include <tuple>

#include <windows.h>
#include <conio.h>

#include <sgr.hpp>

namespace
{
    const int kReductionRate = 2;

    const int kEsc = 27;
    const int kArrowKey = 224;

    const int kArrowUp = 72;
    const int kArrowLeft = 75;
    const int kArrowRight = 77;
    const int kArrowDown = 80;

    const char kEnter = '\r';

    const char kCell[] = "  ";

    // convert colors from an outter representation to inner which we can use to draw.
    // possible overhead.
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
        case Console::Color::ActiveBackground:
            return cpp_sgr::red_bg;
        case Console::Color::Background:
            return cpp_sgr::b_cyan_bg;
        default:
            return cpp_sgr::black_bg;
        }
    }

    // generate a space row by size.
    std::string getSpaces(int size)
    {
        std::string result = "";
        for (int i = 0; i < size; ++i)
        {
            result += kCell;
        }
        return result;
    }
}


Console::Console()
    : consoleHandler(GetStdHandle(STD_OUTPUT_HANDLE))
{
    // anables an ability to draw colors.
    enableVirtualTerminalProcessing();
    showConsoleCursor(false);
    // clears everything just in case.
    clear();
}

Console::~Console()
{
    // clears at the end everything
    clear();
    // restores cursor.
    showConsoleCursor(true);

    // close our handler.
    CloseHandle(consoleHandler);
}

Console::Direction Console::getCurrentDirection()
{
    if (!_kbhit())
    {
        return Direction::Unknown;
    }
 
    auto symbol = _getch();
    switch (symbol)
    {
    case 'a':
        return Direction::Left;
    case 'd':
        return Direction::Right;
    case 'w':
        return Direction::Up;
    case 's':
        return Direction::Down;
    case kArrowKey:
        return getArrowDirection();
    case kEsc:
        return Direction::Esc;
    case kEnter:
        return Direction::Enter;
    default:
        return Direction::Unknown;
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

void Console::drawHorizontalLine(Color color, int x, int y, int size)
{
    auto spaces = getSpaces(size);
    setRealCursorPositon(x, y * kReductionRate);

    std::cerr << convertColor(color) << spaces;
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

// fills background with the special color.
void Console::drawBackground()
{
    int sizeX, sizeY;
    std::tie(sizeX, sizeY) = getScreenSize();

    for (int x = 0; x < sizeX; ++x)
    {
        drawHorizontalLine(Color::Background, x, 0, sizeY);
    }
}

void Console::printLine(Color color, int line,
                        const std::string& text, Color textBackgroundColor, bool isBold)
{
    int _, sizeY;
    std::tie(_, sizeY) = getScreenSize();
    drawHorizontalLine(color, line, 0, sizeY); // draws background

    // finds the text start position.
    int textPosition = (sizeY / 2 - text.size() / (kReductionRate * 2)) * kReductionRate;
    setRealCursorPositon(line, textPosition);
    if (isBold)
    {
        std::cerr << convertColor(textBackgroundColor) << cpp_sgr::bold << text;
        return;
    }
    std::cerr << convertColor(textBackgroundColor) << text;
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
    printf("\x1b[%d;%dH", row + 1, column + 1);
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

    return std::make_tuple(bufferInfo.srWindow.Bottom + 1, bufferInfo.srWindow.Right + 1);
}

Console::Direction Console::getArrowDirection()
{
    if (!_kbhit())
    {
        return Direction::Unknown;
    }

    // additionaly adds <-,-> keys.
    switch (_getch())
    {
    case kArrowLeft:
        return Direction::Left;
    case kArrowRight:
        return Direction::Right;
    case kArrowUp:
        return Direction::Up;
    case kArrowDown:
        return Direction::Down;
    default:
        return Direction::Unknown;
    }
}

void Console::enableVirtualTerminalProcessing()
{
    DWORD mode = 0;
    GetConsoleMode(consoleHandler, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(consoleHandler, mode);

    // References:
    //SetConsoleMode() and ENABLE_VIRTUAL_TERMINAL_PROCESSING?
    //https://stackoverflow.com/questions/38772468/setconsolemode-and-enable-virtual-terminal-processing

    // Windows console with ANSI colors handling
    // https://superuser.com/questions/413073/windows-console-with-ansi-colors-handling
}