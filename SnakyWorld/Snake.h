#pragma once
#include <memory>
#include <deque>

#include "Cell.h"

class Snake
{
public:
    Snake(std::shared_ptr<Console> console, const Coordinate& coordinate);
    ~Snake();

    void move(Console::Directon direction);

private:
    void doStep(Console::Directon direction);

private:
    std::shared_ptr<Console> console;
    std::deque<Cell> cellDeque;

    Coordinate head;
    Console::Directon lastDirection;
};
