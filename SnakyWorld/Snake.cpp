#include "Snake.h"

#include <algorithm>

Snake::Snake(std::shared_ptr<Console> console, const Coordinate& coordinate)
    : console(console), head(coordinate), lastDirection(Console::Directon::Unknown)
{
    for (int i = 0; i < 15; ++i)
    {
        cellDeque.emplace_back(console, head, Console::Color::Yellow);
        ++head.first;
    }
    --head.first;
}

void Snake::move(Console::Directon direction)
{
    switch (direction)
    {
    case Console::Directon::Stop:
        return;

    case Console::Directon::Left:
        --head.second;
        doStep(direction);
        return;

    case Console::Directon::Right:
        ++head.second;
        doStep(direction);
        return;

    case Console::Directon::Up:
        --head.first;
        doStep(direction);
        return;

    case Console::Directon::Down:
        ++head.first;
        doStep(direction);
        return;

    default:
        tryToUseLastStep();
        return;
    }
}

void Snake::setSpawn(std::shared_ptr<Cell> spawn)
{
    this->spawn = spawn;
}

bool Snake::isInteracted(const Coordinate& otherCoordinate) const
{
    int count = std::count_if(std::begin(cellDeque), std::end(cellDeque),
                              [&](const Cell & cell) { return cell.isInteracted(otherCoordinate); });

    if (count > 1)
    {
        return true;
    }

    return false;
}

Coordinate Snake::getCoordindate() const
{
    return head;
}

void Snake::doStep(Console::Directon direction)
{
    lastDirection = direction;

    if (spawn and spawn->isInteracted(head))
    {
        spawn.reset();
    }
    else
    {
        cellDeque.pop_front();
    }

    cellDeque.emplace_back(console, head, Console::Color::Yellow);
}

void Snake::tryToUseLastStep()
{
    if (lastDirection != Console::Directon::Unknown)
    {
        move(lastDirection);
    }
}

Console::Directon Snake::getOppositeDirection(Console::Directon direction)
{
    switch (direction)
    {
    case Console::Directon::Left:
        return Console::Directon::Right;

    case Console::Directon::Right:
        return Console::Directon::Left;

    case Console::Directon::Up:
        return Console::Directon::Down;

    case Console::Directon::Down:
        return Console::Directon::Up;

    default:
        return Console::Directon::Unknown;
    }
}

