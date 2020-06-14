#include "Snake.h"

#include <algorithm>

namespace
{
    Console::Directon getDirectionBaseOnDiff(int diff, Console::Directon first, Console::Directon second)
    {
        if (diff < 0)
        {
            return first;
        }
 
        if (diff > 0)
        {
            return second;
        }

        return Console::Directon::Unknown;
    }
}

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
    auto directions = getPossibleDirections();
    auto coordinate = moveCoordinate(head, directions.first);

    int countFirst = std::count_if(std::begin(cellDeque), std::end(cellDeque),
        [&](const Cell& cell) { return cell.isInteracted(coordinate); });

    if (countFirst == 0)
    {
        move(directions.first);
    }
    else
    {
        move(directions.second);
    }
}

void Snake::move(Console::Directon direction)
{
    auto oppositeDirection = getOppositeDirection(lastDirection);
    if (direction == oppositeDirection)
    {
        tryToUseLastStep();
        return;
    }

    switch (direction)
    {
    case Console::Directon::Left:
    case Console::Directon::Right:
    case Console::Directon::Up:
    case Console::Directon::Down:
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
    int count = std::count_if(std::begin(cellDeque), std::end(cellDeque) - 1,
                              [&](const Cell & cell) { return cell.isInteracted(otherCoordinate); });
    return count != 0;
}

Coordinate Snake::getCoordindate() const
{
    return head;
}

void Snake::doStep(Console::Directon direction)
{
    head = moveCoordinate(head, direction);
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

PossibleDirections Snake::getPossibleDirections()
{
    auto destination = spawn->getCoordindate();
    int xDiff = destination.first - head.first;
    int yDiff = destination.second - head.second;

    auto horizontalDirection = getDirectionBaseOnDiff(yDiff,
                                                      Console::Directon::Left,
                                                      Console::Directon::Right);
    auto verticalDirection = getDirectionBaseOnDiff(xDiff,
                                                    Console::Directon::Up,
                                                    Console::Directon::Down);

    auto oppositeDirection = getOppositeDirection(lastDirection);
    if (horizontalDirection == oppositeDirection)
    {
        horizontalDirection = Console::Directon::Down;
    }

    if (verticalDirection == oppositeDirection)
    {
        verticalDirection = Console::Directon::Right;
    }

    return PossibleDirections(horizontalDirection, verticalDirection);
}

Coordinate Snake::moveCoordinate(const Coordinate& coordinate, Console::Directon direction)
{
    switch (direction)
    {
    case Console::Directon::Left:
        return Coordinate(coordinate.first, coordinate.second - 1);

    case Console::Directon::Right:
        return Coordinate(coordinate.first, coordinate.second + 1);

    case Console::Directon::Up:
        return Coordinate(coordinate.first - 1, coordinate.second);

    case Console::Directon::Down:
        return Coordinate(coordinate.first + 1, coordinate.second);

    default:
        return coordinate;
    }
}
