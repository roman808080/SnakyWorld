#include "Snake.h"

Snake::Snake(std::shared_ptr<Console> console, const Coordinate& coordinate)
    : console(console)
{
    cellDeque.emplace_back(console, coordinate, Console::Color::Yellow);
}


Snake::~Snake()
{
}
