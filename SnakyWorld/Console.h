#pragma once

#include <tuple>
#include <windows.h>

// Console is responsible for low level interaction with a console.
class Console
{
    enum class Directon
    {
        STOP,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        UNDEFINED // If was pressed a different key.
    };

public:
    Console();
    ~Console();

    Directon getCurrentDirection();

    // Returns a formatted screen size. The x axe is reduced due to uneven rectangles.
    std::tuple<int, int> getScreenSize();

    // Draws a ceil.
    // In reality, it draws two ceils because the size in the y-direction is doubled compared to the x-direction.
    void drawCeil(int x, int y);

    // Clears the screen.
    void clear();

private:
    // It is used to hide the cursor.
    void showConsoleCursor(bool showFlag);

    // It sets to put the cursor in a real console position.
    void setRealCursorPositon(int x, int y);

    // It is used to get a real size without a reduction in the x axis.
    std::tuple<int, int> getRealScreenSize();

private:
    HANDLE consoleHandler;
};
