#include "Line.h"
#include <stdexcept>


Line::Line(std::shared_ptr<Console> console, const Coordinate& startCoordinate, const Coordinate& endCoordinate)
    : console(console), startCoordinate(startCoordinate), endCoordinate(endCoordinate)
{
    isHorizontal = isLineHorizontal();
    draw(Console::Color::Red);
}

Interaction Line::isInteracted(const ConsoleObject& consoleObject)
{
    return Interaction();
}

Coordinate Line::getCoordindate()
{
    return startCoordinate;
}

bool Line::isLineHorizontal()
{
    if (startCoordinate.first != endCoordinate.first and
        startCoordinate.second == endCoordinate.second)
    {
        return false;
    }
    else if (startCoordinate.first == endCoordinate.first and
             startCoordinate.second != endCoordinate.second)
    {
        return true;
    }
    else
    {
        throw std::runtime_error("Incorrect format.");
    }
}

void Line::draw(Console::Color color)
{
    auto currentCoordinate = startCoordinate;
    while (currentCoordinate != endCoordinate)
    {
        console->drawCell(color, currentCoordinate.first, currentCoordinate.second);
        if (isHorizontal)
        {
            ++currentCoordinate.second;
        }
        else
        {
            ++currentCoordinate.first;
        }
    }
}

Line::~Line()
{
    draw(Console::Color::Black);
}
