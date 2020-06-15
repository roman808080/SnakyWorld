#pragma once
#include <memory>
#include <deque>
#include <vector>
#include <future>

#include "Cell.h"
#include "ConsumptionObserver.h"

// the main character and my personal hero - the snake.
class Snake: public ConsoleObject
{
public:
    Snake(std::shared_ptr<Console> console, const Coordinate& coordinate, int size = 3);

    // automated movement.
    void move();
    // user based movement.
    void move(Console::Direction direction);

    // we need to know where borders are. it is important when we are in the demo mode.
    void setBorders(const Coordinate& topLeftCorner, const Coordinate& bottomRightCorner);
    // says where our victim is based.
    void setSpawn(std::shared_ptr<Cell> spawn);
    // sets where we need to send our events.
    void setConsumptionObserver(ConsumptionObserver* consumptionObserver);

    // checks interactions with inner cells.
    virtual bool isInteracted(const Coordinate& otherCoordinate) const override;
    // returns the coordinate of the current head.
    virtual Coordinate getCoordindate() const override;

private:
    // move one step further.
    void doStep(Console::Direction direction);
    // if we failed to use the current input, try to use previous one.
    void tryToUseLastStep();

    // returns a direction which is opposite to the current one.
    // we need the method to check that we are not moving back to our neck.
    Console::Direction getOppositeDirection(Console::Direction direction);
    // returns 6 directions. the first two are the best scenarios for move, the rest are just default options.
    std::vector<Console::Direction> getPossibleDirections(const Coordinate& snakeHead,
                                                         const Coordinate& destination);
    // recursively calculate the next step for demo. the algorithm has problems and bugs.
    // also recursion possibly doesn't work properly. needs an additional work.
    Console::Direction calculateDirection(const Coordinate& snakeHead, const Coordinate& spawnCoordinate,
                                        Console::Direction previousDirection = Console::Direction::Unknown,
                                        int depth = 40);
    // calculate the next position which based on previous and direction to a new one.
    Coordinate moveCoordinate(const Coordinate& coordinate,
                              Console::Direction direction);
    // checks possibility of collision with a border.
    bool isBorderCollision(const Coordinate& otherCoordinate);

    // start async the calculation of a new position.
    void calculationRequest();

private:
    std::shared_ptr<Console> console;
    std::shared_ptr<Cell> spawn; // our victim)

    // our snake consists of separete cells which were organised in the supreme form of life.
    std::deque<Cell> cellDeque;

    // coordinates of the current head.
    Coordinate head;

    // borders coordinates.
    Coordinate topLeftCorner;
    Coordinate bottomRightCorner;

    // needs in some cases to just keep going
    Console::Direction lastDirection;
    // future of our next move
    std::future<Console::Direction> nextDirection;

    // a pointer to the board to notify about events.
    ConsumptionObserver* consumptionObserver;
};
