#pragma once
#include <memory>

#include "ConsoleObject.h"
#include "Console.h"

// draws line from start to end point.
class Line: public ConsoleObject
{
public:
    Line(std::shared_ptr<Console> console, const Coordinate& startCoordinate, const Coordinate& endCoordinate);

    // checks interactions with other objects.
    virtual bool isInteracted(const Coordinate& otherCoordinate) const override;

    // returns start position, maybe needs refactoring.
    virtual Coordinate getCoordindate() const override;

    virtual ~Line();

private:
    // detects which line we need to draw horizontal or vertical.
    bool isLineHorizontal();

    // draws itself.
    void draw(Console::Color color);

private:
    std::shared_ptr<Console> console;

    Coordinate startCoordinate;
    Coordinate endCoordinate;

    bool isHorizontal;
};
