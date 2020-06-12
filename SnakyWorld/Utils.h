#pragma once

#include <tuple>
#include <windows.h>

class Console
{
    enum class Directon
    {
        STOP,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        UNDEFINED
    };

public:
    Console();
    ~Console();

    Directon getCurrentDirection();
    std::tuple<int, int> getScreenSize();
    void drawCeil(int x, int y);
    void clear();

private:
    void showConsoleCursor(bool showFlag);
    void setRealCursorPositon(int x, int y);
    std::tuple<int, int> getRealScreenSize();

private:
    HANDLE consoleHandler;
};
