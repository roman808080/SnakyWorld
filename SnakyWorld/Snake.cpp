#include "Snake.h"

#include <algorithm>

Snake::Snake(std::shared_ptr<Console> console, const Coordinate& coordinate)
    : console(console), head(coordinate), lastDirection(Console::Directon::Unknown)
{
    cellDeque.emplace_back(console, coordinate, Console::Color::Yellow);
}

void Snake::move(Console::Directon direction)
{
    switch (direction)
    {
    case Console::Directon::Stop:
        return;

    case Console::Directon::Left:
        --head.second;
        doStep(direction);
        return;

    case Console::Directon::Right:
        ++head.second;
        doStep(direction);
        return;

    case Console::Directon::Up:
        --head.first;
        doStep(direction);
        return;

    case Console::Directon::Down:
        ++head.first;
        doStep(direction);
        return;

    default:
        tryToUseLastStep();
        return;
    }
}

Interaction Snake::isInteracted(const Coordinate& otherCoordinate) const
{
    auto result = std::find_if(std::begin(cellDeque), std::end(cellDeque),
                               [&](const Cell & cell)
        { return cell.isInteracted(otherCoordinate) == Interaction::Collided; });

    if (result != std::end(cellDeque))
    {
        return Interaction::Collided;
    }

    return Interaction::NoInteraction;
}

Coordinate Snake::getCoordindate() const
{
    return head;
}

void Snake::doStep(Console::Directon direction)
{
    lastDirection = direction;
    cellDeque.emplace_back(console, head, Console::Color::Yellow);
    cellDeque.pop_front();
}

void Snake::tryToUseLastStep()
{
    if (lastDirection != Console::Directon::Unknown)
    {
        move(lastDirection);
    }
}

Snake::~Snake()
{
}
