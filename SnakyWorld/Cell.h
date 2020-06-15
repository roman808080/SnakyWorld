#pragma once
#include <memory>

#include "ConsoleObject.h"
#include "Console.h"

// the smallest primitive in the game. from these blocks are built vertical walls and a snake.
class Cell: public ConsoleObject
{
public:
    Cell(std::shared_ptr<Console> console, const Coordinate& coordinate,
         Console::Color color = Console::Color::Green);

    // checks collisions
    virtual bool isInteracted(const Coordinate& otherCoordinate) const override;

    // returns the block possition.
    virtual Coordinate getCoordindate() const override;

    virtual ~Cell();

protected:
    Coordinate coordinate;

private:
    std::shared_ptr<Console> console;
};

