#pragma once
#include <memory>

#include "ConsoleObject.h"
#include "Console.h"

class Cell: public ConsoleObject
{
public:
    Cell(std::shared_ptr<Console> console, const Coordinate& coordinate,
         Console::Color color = Console::Color::Green);

    virtual Interaction isInteracted(const Coordinate& otherCoordinate) const override;
    virtual Coordinate getCoordindate() const override;

    virtual ~Cell();

private:
    Coordinate coordinate;
    std::shared_ptr<Console> console;
};

