#pragma once
#include <memory>
#include <deque>

#include "Cell.h"

class Snake: public ConsoleObject
{
public:
    Snake(std::shared_ptr<Console> console, const Coordinate& coordinate);
    virtual ~Snake();

    void move(Console::Directon direction);

    virtual bool isInteracted(const Coordinate& otherCoordinate) const override;
    virtual Coordinate getCoordindate() const override;

private:
    void doStep(Console::Directon direction);
    void tryToUseLastStep();

private:
    std::shared_ptr<Console> console;
    std::deque<Cell> cellDeque;

    Coordinate head;
    Console::Directon lastDirection;
};
