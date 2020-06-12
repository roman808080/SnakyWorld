#include "Cell.h"

Cell::Cell(std::shared_ptr<Console> console, const Coordinate& coordinate,
           Console::Color color)
    : console(console), coordinate(coordinate)
{
    console->drawCell(color, this->coordinate.first, this->coordinate.second);
}

Interaction Cell::isInteracted(const ConsoleObject& consoleObject)
{
    return Interaction();
}

Coordinate Cell::getCoordindate()
{
    return coordinate;
}

Cell::~Cell()
{
    console->drawCell(Console::Color::Black, this->coordinate.first, this->coordinate.second);
}
