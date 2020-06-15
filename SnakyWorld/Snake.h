#pragma once
#include <memory>
#include <deque>
#include <vector>
#include <future>

#include "Cell.h"
#include "ConsumptionObserver.h"


class Snake: public ConsoleObject
{
public:
    Snake(std::shared_ptr<Console> console, const Coordinate& coordinate, int size = 3);

    void move();
    void move(Console::Direction direction);

    void setBorders(const Coordinate& topLeftCorner, const Coordinate& bottomRightCorner);
    void setSpawn(std::shared_ptr<Cell> spawn);
    void setConsumptionObserver(ConsumptionObserver* consumptionObserver);

    virtual bool isInteracted(const Coordinate& otherCoordinate) const override;
    virtual Coordinate getCoordindate() const override;

private:
    void doStep(Console::Direction direction);
    void tryToUseLastStep();

    Console::Direction getOppositeDirection(Console::Direction direction);
    std::vector<Console::Direction> getPossibleDirections(const Coordinate& snakeHead,
                                                         const Coordinate& destination);
    Console::Direction calculateDirection(const Coordinate& snakeHead, const Coordinate& spawnCoordinate,
                                        Console::Direction previousDirection = Console::Direction::Unknown,
                                        int depth = 40);
    Coordinate moveCoordinate(const Coordinate& coordinate,
                              Console::Direction direction);
    bool isBorderCollision(const Coordinate& otherCoordinate);
    void calculationRequest();

private:
    std::shared_ptr<Console> console;
    std::shared_ptr<Cell> spawn;

    std::deque<Cell> cellDeque;

    Coordinate head;
    Coordinate topLeftCorner;
    Coordinate bottomRightCorner;

    Console::Direction lastDirection;
    std::future<Console::Direction> nextDirection;

    ConsumptionObserver* consumptionObserver;
};
