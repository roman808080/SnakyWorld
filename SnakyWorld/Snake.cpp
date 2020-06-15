#include "Snake.h"

#include <algorithm>

namespace
{
    // this function just to reduce duplication.
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

    // reverse directions base on input.
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

    // build the snake starting from the head.
    for (int i = 0; i < size; ++i)
    {
        cellDeque.emplace_front(console, teil, Console::Color::Yellow);
        --teil.second;
    }
}

void Snake::move()
{
    // get precalculated result.
    move(nextDirection.get());
    // to request a new calculation in another thread.
    calculationRequest();
}

void Snake::move(Console::Direction direction)
{
    // if we try to move back towards our body, the snake just ignores this and continue our path in the universe.
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
        // ignore a bad keyboard input.
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
    // set our target.
    this->spawn = spawn;

    // request calculation to be ready after we will awake.
    calculationRequest();
}

void Snake::setConsumptionObserver(ConsumptionObserver* consumptionObserver)
{
    this->consumptionObserver = consumptionObserver;
}

bool Snake::isInteracted(const Coordinate& otherCoordinate) const
{
    // check collisions with all cells except the head,
    // because we receive a head coordinations and to compare the head with itself is a bad idea.
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
    // move forward
    head = moveCoordinate(head, direction);
    lastDirection = direction;

    // check if we get the target or not.
    if (spawn and spawn->isInteracted(head))
    {
        // destroy the current spawn.
        spawn.reset();
        if (consumptionObserver != nullptr)
        {
            // notify that we need a new one.
            consumptionObserver->spawnWasEaten();
        }
    }
    else
    {
        // reduce the tail if we are still hungry
        cellDeque.pop_front();
    }

    // add a new cell to the top.
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
    // if no previous directions assume that it right. there is a possiblity of bugs.
    if (lastDirection == Console::Direction::Unknown)
    {
        return Console::Direction::Left;
    }

    // otherwise left becomes right, up becomes down and vice versa.
    return reverseDirection(direction);
}

std::vector<Console::Direction> Snake::getPossibleDirections(const Coordinate& snakeHead,
                                                             const Coordinate& destination)
{
    // try to find out how many steps we need to do in each axis to get the target.
    int xDiff = destination.first - snakeHead.first;
    int yDiff = destination.second - snakeHead.second;

    // generate horizontal hint.
    auto horizontalDirection = getDirectionBaseOnDiff(yDiff,
                                                      Console::Direction::Left,
                                                      Console::Direction::Right);
    // generate vertical hint.
    auto verticalDirection = getDirectionBaseOnDiff(xDiff,
                                                    Console::Direction::Up,
                                                    Console::Direction::Down);

    // there is a possibility that it is impossible to follow the hints in this case just use any direction.
    // there is a duplication with directions. needs refactoring.
    std::vector<Console::Direction> directions{ horizontalDirection, verticalDirection,
                                               Console::Direction::Left, Console::Direction::Right,
                                               Console::Direction::Up, Console::Direction::Down };

    return directions;
}

// this algorithm is quite bad for now. there are parts which do not work properly.
// Also, maybe I need just to rewrite the code and use some sort of shortest path algorihtm.
Console::Direction Snake::calculateDirection(const Coordinate& snakeHead, const Coordinate& spawnCoordinate,
                                            Console::Direction previousDirection, int depth)

{
    // I added a recursion to try to forecast collisions in some directions. to think slightly ahead.
    // For now, it doesn't work properly and has its own issues.
    // Maybe I would like to enhance these methods in the future.

    // if snakeHead == spawnCoordinate than we already get our way.
    // depth - to ensure the end of the recursion. There is no guarantee for that otherwise.
    if (snakeHead == spawnCoordinate or
        depth <= 0) 
    {
        return previousDirection;
    }

    auto directions = getPossibleDirections(snakeHead, spawnCoordinate);

    for (const auto direction : directions)
    {
        // check if the direction is useless. if true than skip.
        if (direction == Console::Direction::Unknown)
        {
            continue;
        }

        // calculate a new coordinate
        auto coordinate = moveCoordinate(head, direction);
        // checks if it has collisions with snake's body or borders.
        if (!isInteracted(coordinate) and !isBorderCollision(coordinate))
        {
            return calculateDirection(coordinate, spawnCoordinate, direction, depth - 1);
        }
    }

    // WE ARE DOOMED)
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
    // assumes that borders are infinite in their axis.
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

    // the algorithm can take a while time, at least theoretically.
    // for now, it is more artificially at least in case when recursion is turn off.
    nextDirection = std::async(request, head, spawn->getCoordindate());
}
