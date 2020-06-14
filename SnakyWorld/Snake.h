#pragma once
#include <memory>
#include <deque>

#include "Cell.h"
#include "ConsumptionObserver.h"

class Snake: public ConsoleObject
{
public:
    Snake(std::shared_ptr<Console> console, const Coordinate& coordinate, int size = 3);

    void move();
    void move(Console::Directon direction);
    void setSpawn(std::shared_ptr<Cell> spawn);
    void setConsumptionObserver(ConsumptionObserver* consumptionObserver);

    virtual bool isInteracted(const Coordinate& otherCoordinate) const override;
    virtual Coordinate getCoordindate() const override;

private:
    void doStep(Console::Directon direction);
    void tryToUseLastStep();
    Console::Directon getOppositeDirection(Console::Directon direction);

private:
    std::shared_ptr<Console> console;
    std::shared_ptr<Cell> spawn;
    std::deque<Cell> cellDeque;

    Coordinate head;
    Console::Directon lastDirection;

    ConsumptionObserver* consumptionObserver;
};
