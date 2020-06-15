#pragma once

#include <tuple>
#include <string>

#include <windows.h>

// Console is responsible for low level interaction with a console.
// The object is omnipresent, maybe needs to be singleton, but I am not sure about this.

class Console
{
public:

    // represents directions and special keys.
    enum class Direction
    {
        Left,
        Right,
        Up,
        Down,
        Esc,
        Enter,
        Unknown // If was pressed a different key.
    };

    // colors which I used.
    enum class Color
    {
        Black,
        Green,
        Red,
        Yellow,
        ActiveBackground,
        Background,
        Unknown
    };

    Console();
    ~Console();

    // Returns current direction.
    Direction getCurrentDirection();

    // Returns a formatted screen size. The y axe is reduced due to uneven rectangles.
    std::tuple<int, int> getScreenSize();

    // Draws a cell.
    // In reality, it draws two cells because the size in the x-direction is doubled compared to the y-direction.
    void drawCell(Color color, int x, int y);

    // Draws a horizontal line.
    void drawHorizontalLine(Color color, int x, int y, int size);

    // Clears the screen.
    void clear();

    // Fills the screen with a background color.
    void drawBackground();

    // print a text line which centred. possibly too many parameters, maybe needs refactoring.
    void printLine(Color color, int line,
                   const std::string& text, Color textBackgroundColor, bool isBold);

private:
    // It is used to hide the cursor.
    void showConsoleCursor(bool showFlag);

    // It sets to put the cursor in a real console position.
    void setRealCursorPositon(int x, int y);

    // It is used to get a real size without a reduction in the y axis.
    std::tuple<int, int> getRealScreenSize();

    // In case arrow keys were used.
    Direction getArrowDirection();

    // Fixes an issue on Windows 10 with ANSI symbols.
    void enableVirtualTerminalProcessing();

private:
    HANDLE consoleHandler;
};
