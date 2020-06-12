#include "Snake.h"

Snake::Snake(std::shared_ptr<Console> console, const Coordinate& coordinate)
    : console(console), head(coordinate), lastDirection(Console::Directon::Unknown)
{
    cellDeque.emplace_back(console, coordinate, Console::Color::Yellow);
}

void Snake::move(Console::Directon direction)
{
    switch (direction)
    {
    case Console::Directon::Stop:
        return;

    case Console::Directon::Left:
        --head.first;
        doStep(direction);
        return;

    case Console::Directon::Right:
        ++head.first;
        doStep(direction);
        return;

    case Console::Directon::Up:
        --head.second;
        doStep(direction);
        return;

    case Console::Directon::Down:
        ++head.second;
        doStep(direction);
        return;

    default:
        tryToUseLastStep();
        return;
    }
}

void Snake::doStep(Console::Directon direction)
{
    lastDirection = direction;
    cellDeque.emplace_back(console, head, Console::Color::Yellow);
    cellDeque.pop_front();
}

void Snake::tryToUseLastStep()
{
    if (lastDirection != Console::Directon::Unknown)
    {
        move(lastDirection);
    }
}

Snake::~Snake()
{
}
