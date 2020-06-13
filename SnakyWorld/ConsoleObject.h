#pragma once
#include <utility>

using Coordinate = std::pair<int, int>;

class ConsoleObject
{
public:
    virtual bool isInteracted(const Coordinate& otherCoordinate) const = 0;
    virtual Coordinate getCoordindate() const = 0;

    virtual ~ConsoleObject() {};
};
