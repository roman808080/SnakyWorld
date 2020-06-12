#include "Ceil.h"


Ceil::Ceil(std::shared_ptr<Console> console, Coordinate coordinate)
    : console(console), coordinate(coordinate)
{
    console->drawCeil(this->coordinate.first, this->coordinate.second);
}

Interaction Ceil::isInteracted(const ConsoleObject& consoleObject)
{
    return Interaction();
}

Coordinate Ceil::getCoordindate()
{
    return coordinate;
}

Ceil::~Ceil()
{
}
