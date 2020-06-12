#pragma once
#include <memory>
#include <deque>

#include "Cell.h"

class Snake
{
public:
    Snake(std::shared_ptr<Console> console, const Coordinate& coordinate);
    ~Snake();

private:
    std::shared_ptr<Console> console;
    std::deque<Cell> cellDeque;
};
