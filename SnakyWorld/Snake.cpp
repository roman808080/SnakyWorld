#include "Snake.h"

#include <algorithm>

Snake::Snake(std::shared_ptr<Console> console, const Coordinate& coordinate, int size)
    : console(console), head(coordinate), lastDirection(Console::Directon::Unknown),
      consumptionObserver(nullptr)
{
    auto teil = head;
    for (int i = 0; i < size; ++i)
    {
        cellDeque.emplace_front(console, teil, Console::Color::Yellow);
        --teil.second;
    }
}

void Snake::move()
{
    auto destination = spawn->getCoordindate();
    int xDiff = destination.first - head.first;
    int yDiff = destination.second - head.second;

    if (xDiff > 0)
    {
        move(Console::Directon::Down);
        return;
    }

    if (xDiff < 0)
    {
        move(Console::Directon::Up);
        return;
    }

    if (yDiff < 0)
    {
        move(Console::Directon::Left);
        return;
    }

    if (yDiff > 0)
    {
        move(Console::Directon::Right);
        return;
    }

    move(Console::Directon::Unknown);
    return;
}

void Snake::move(Console::Directon direction)
{
    auto oppositeDirection = getOppositeDirection(lastDirection);
    if (direction == getOppositeDirection(lastDirection))
    {
        tryToUseLastStep();
        return;
    }

    switch (direction)
    {
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

void Snake::setConsumptionObserver(ConsumptionObserver* consumptionObserver)
{
    this->consumptionObserver = consumptionObserver;
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
        if (consumptionObserver != nullptr)
        {
            consumptionObserver->spawnWasEaten();
        }
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
    if (lastDirection == Console::Directon::Unknown)
    {
        return Console::Directon::Left;
    }

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
