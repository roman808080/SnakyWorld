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

    Console::Directon reverseDirection(Console::Directon direction)
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
    auto direction = calculateDirection(head, spawn->getCoordindate());
    move(direction);
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
    return reverseDirection(direction);
}

std::vector<Console::Directon> Snake::getPossibleDirections(const Coordinate& snakeHead,
                                                            const Coordinate& destination)
{
    int xDiff = destination.first - snakeHead.first;
    int yDiff = destination.second - snakeHead.second;

    auto horizontalDirection = getDirectionBaseOnDiff(yDiff,
                                                      Console::Directon::Left,
                                                      Console::Directon::Right);
    auto verticalDirection = getDirectionBaseOnDiff(xDiff,
                                                    Console::Directon::Up,
                                                    Console::Directon::Down);

    std::vector<Console::Directon> directions{ horizontalDirection, verticalDirection,
                                               reverseDirection(horizontalDirection),
                                               reverseDirection(verticalDirection) };

    return directions;
}

Console::Directon Snake::calculateDirection(const Coordinate& snakeHead, const Coordinate& spawnCoordinate)
{
    auto directions = getPossibleDirections(snakeHead, spawnCoordinate);
    for (const auto direction : directions)
    {
        auto coordinate = moveCoordinate(head, direction);
        int count = std::count_if(std::begin(cellDeque), std::end(cellDeque),
            [&](const Cell & cell) { return cell.isInteracted(coordinate); });
        if (count == 0)
        {
            return direction;
        }
    }

    return Console::Directon::Unknown;
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
