#pragma once
#include <utility>

enum class Interaction
{
    NoInteraction,
    Collided,
    Consumed
};

using Coordinate = std::pair<int, int>;

class ConsoleObject
{
public:
    virtual Interaction isInteracted(const Coordinate& otherCoordinate) const = 0;
    virtual Coordinate getCoordindate() const = 0;

    virtual ~ConsoleObject() {};
};
