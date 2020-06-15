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
    : console(console), head(coordinate), topLeftCorner(0, 0), bottomRightCorner(0, 0),
      lastDirection(Console::Directon::Unknown), consumptionObserver(nullptr)
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
    move(nextDirection.get());

    nextDirection = std::async([&](Coordinate first, Coordinate second) {
        return calculateDirection(first, second);
        }, head, spawn->getCoordindate());
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

void Snake::setBorders(const Coordinate& topLeftCorner, const Coordinate& bottomRightCorner)
{
    this->topLeftCorner = topLeftCorner;
    this->bottomRightCorner = bottomRightCorner;
}

void Snake::setSpawn(std::shared_ptr<Cell> spawn)
{
    this->spawn = spawn;

    nextDirection = std::async([&] (Coordinate first, Coordinate second) {
            return calculateDirection(first, second);
        }, head, spawn->getCoordindate());
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
                                               Console::Directon::Left, Console::Directon::Right,
                                               Console::Directon::Up, Console::Directon::Down };

    return directions;
}

Console::Directon Snake::calculateDirection(const Coordinate& snakeHead, const Coordinate& spawnCoordinate,
                                            Console::Directon previousDirection, int depth)
{
    if (snakeHead == spawnCoordinate or
        depth <= 0)
    {
        return previousDirection;
    }

    auto directions = getPossibleDirections(snakeHead, spawnCoordinate);

    for (const auto direction : directions)
    {
        if (direction == Console::Directon::Unknown)
        {
            continue;
        }
        auto coordinate = moveCoordinate(head, direction);
        if (!isInteracted(coordinate) and !isBorderCollision(coordinate))
        {
            return calculateDirection(coordinate, spawnCoordinate, direction, depth - 1);
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

bool Snake::isBorderCollision(const Coordinate& otherCoordinate)
{
    return (otherCoordinate.first == topLeftCorner.first or
            otherCoordinate.first == bottomRightCorner.first or
            otherCoordinate.second == topLeftCorner.second or
            otherCoordinate.second == bottomRightCorner.second);
}
