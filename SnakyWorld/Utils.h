#pragma once

#include <tuple>

namespace Utils
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

    void setCursorPositon(int x, int y);
    void showConsoleCursor(bool showFlag);

    Directon getCurrentDirection();
    void clearConslole();

    std::tuple<int, int, int, int> getScreenCoordinates();

    class Console
    {
       Console(){}
    };
};
