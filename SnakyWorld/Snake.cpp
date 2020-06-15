#include "Snake.h"

#include <algorithm>

namespace
{
    Console::Direction getDirectionBaseOnDiff(int diff, Console::Direction first, Console::Direction second)
    {
        if (diff < 0)
        {
            return first;
        }
 
        if (diff > 0)
        {
            return second;
        }

        return Console::Direction::Unknown;
    }

    Console::Direction reverseDirection(Console::Direction direction)
    {
        switch (direction)
        {
        case Console::Direction::Left:
            return Console::Direction::Right;

        case Console::Direction::Right:
            return Console::Direction::Left;

        case Console::Direction::Up:
            return Console::Direction::Down;

        case Console::Direction::Down:
            return Console::Direction::Up;

        default:
            return Console::Direction::Unknown;
        }
    }
}

Snake::Snake(std::shared_ptr<Console> console, const Coordinate& coordinate, int size)
    : console(console), head(coordinate), topLeftCorner(0, 0), bottomRightCorner(0, 0),
      lastDirection(Console::Direction::Unknown), consumptionObserver(nullptr)
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
    calculationRequest();
}

void Snake::move(Console::Direction direction)
{
    auto oppositeDirection = getOppositeDirection(lastDirection);
    if (direction == oppositeDirection)
    {
        tryToUseLastStep();
        return;
    }

    switch (direction)
    {
    case Console::Direction::Left:
    case Console::Direction::Right:
    case Console::Direction::Up:
    case Console::Direction::Down:
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
    calculationRequest();
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

void Snake::doStep(Console::Direction direction)
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
    if (lastDirection != Console::Direction::Unknown)
    {
        move(lastDirection);
    }
}

Console::Direction Snake::getOppositeDirection(Console::Direction direction)
{
    if (lastDirection == Console::Direction::Unknown)
    {
        return Console::Direction::Left;
    }
    return reverseDirection(direction);
}

std::vector<Console::Direction> Snake::getPossibleDirections(const Coordinate& snakeHead,
                                                            const Coordinate& destination)
{
    int xDiff = destination.first - snakeHead.first;
    int yDiff = destination.second - snakeHead.second;

    auto horizontalDirection = getDirectionBaseOnDiff(yDiff,
                                                      Console::Direction::Left,
                                                      Console::Direction::Right);
    auto verticalDirection = getDirectionBaseOnDiff(xDiff,
                                                    Console::Direction::Up,
                                                    Console::Direction::Down);

    std::vector<Console::Direction> directions{ horizontalDirection, verticalDirection,
                                               Console::Direction::Left, Console::Direction::Right,
                                               Console::Direction::Up, Console::Direction::Down };

    return directions;
}

Console::Direction Snake::calculateDirection(const Coordinate& snakeHead, const Coordinate& spawnCoordinate,
                                            Console::Direction previousDirection, int depth)
{
    if (snakeHead == spawnCoordinate or
        depth <= 0)
    {
        return previousDirection;
    }

    auto directions = getPossibleDirections(snakeHead, spawnCoordinate);

    for (const auto direction : directions)
    {
        if (direction == Console::Direction::Unknown)
        {
            continue;
        }
        auto coordinate = moveCoordinate(head, direction);
        if (!isInteracted(coordinate) and !isBorderCollision(coordinate))
        {
            return calculateDirection(coordinate, spawnCoordinate, direction, depth - 1);
        }
    }

    return Console::Direction::Unknown;
}

Coordinate Snake::moveCoordinate(const Coordinate& coordinate, Console::Direction direction)
{
    switch (direction)
    {
    case Console::Direction::Left:
        return Coordinate(coordinate.first, coordinate.second - 1);

    case Console::Direction::Right:
        return Coordinate(coordinate.first, coordinate.second + 1);

    case Console::Direction::Up:
        return Coordinate(coordinate.first - 1, coordinate.second);

    case Console::Direction::Down:
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

void Snake::calculationRequest()
{
    // calling a private method because of this in lambda.
    auto request = [&](Coordinate first, Coordinate second) 
                      {
                          return calculateDirection(first, second);
                      };

    nextDirection = std::async(request, head, spawn->getCoordindate());
}
