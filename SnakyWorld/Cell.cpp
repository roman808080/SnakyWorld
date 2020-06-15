#include "Cell.h"

Cell::Cell(std::shared_ptr<Console> console, const Coordinate& coordinate,
           Console::Color color)
    : console(console), coordinate(coordinate)
{
    // we draw the cell during the init process and draw background when we die.
    console->drawCell(color, this->coordinate.first, this->coordinate.second);
}

bool Cell::isInteracted(const Coordinate& otherCoordinate) const
{
    // a simple check of collision.
    if (otherCoordinate == coordinate)
    {
        return true;
    }
    return false;
}

Coordinate Cell::getCoordindate() const
{
    return coordinate;
}

Cell::~Cell()
{
    console->drawCell(Console::Color::Background, this->coordinate.first, this->coordinate.second);
}
