#include "Cell.h"

Cell::Cell(std::shared_ptr<Console> console, const Coordinate& coordinate,
           Console::Color color)
    : console(console), coordinate(coordinate)
{
    console->drawCell(color, this->coordinate.first, this->coordinate.second);
}

Interaction Cell::isInteracted(const Coordinate& otherCoordinate) const
{
    if (otherCoordinate == coordinate)
    {
        return Interaction::Collided;
    }
    return Interaction::NoInteraction;
}

Coordinate Cell::getCoordindate() const
{
    return coordinate;
}

Cell::~Cell()
{
    console->drawCell(Console::Color::Black, this->coordinate.first, this->coordinate.second);
}
