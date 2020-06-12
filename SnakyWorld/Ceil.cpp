#include "Ceil.h"

Ceil::Ceil(std::shared_ptr<Console> console, const Coordinate& coordinate)
    : console(console), coordinate(coordinate)
{
    console->drawCeil(Console::Color::Green, this->coordinate.first, this->coordinate.second);
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
    console->drawCeil(Console::Color::Black, this->coordinate.first, this->coordinate.second);
}
