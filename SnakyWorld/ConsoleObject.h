#pragma once
#include <utility>

using Coordinate = std::pair<int, int>;

// abstraction for representation of all objects which could be drawn (lines, cells, snakes).
class ConsoleObject
{
public:
    // checks fact of interaction with something.
    virtual bool isInteracted(const Coordinate& otherCoordinate) const = 0;

    // returns coordinates
    virtual Coordinate getCoordindate() const = 0;

    virtual ~ConsoleObject() {};
};
