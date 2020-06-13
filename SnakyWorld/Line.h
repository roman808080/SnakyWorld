#pragma once
#include <memory>

#include "ConsoleObject.h"
#include "Console.h"

class Line: public ConsoleObject
{
public:
    Line(std::shared_ptr<Console> console, const Coordinate& startCoordinate, const Coordinate& endCoordinate);

    virtual bool isInteracted(const Coordinate& otherCoordinate) const override;
    virtual Coordinate getCoordindate() const override;

    virtual ~Line();

private:
    bool isLineHorizontal();
    void draw(Console::Color color);

private:
    std::shared_ptr<Console> console;

    Coordinate startCoordinate;
    Coordinate endCoordinate;

    bool isHorizontal;
};
