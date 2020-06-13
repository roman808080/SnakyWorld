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

void Snake::setSpawn(std::shared_ptr<Cell> spawn)
{
    this->spawn = spawn;
}

bool Snake::isInteracted(const Coordinate& otherCoordinate) const
{
    int count = std::count_if(std::begin(cellDeque), std::end(cellDeque),
                              [&](const Cell & cell) { return cell.isInteracted(otherCoordinate); });

    if (count > 1)
    {
        return true;
    }

    return false;
}

Coordinate Snake::getCoordindate() const
{
    return head;
}

void Snake::doStep(Console::Directon direction)
{
    lastDirection = direction;
    cellDeque.emplace_back(console, head, Console::Color::Yellow);
    
    if (spawn and spawn->isInteracted(head))
    {
        spawn.reset();
        return;
    }

    cellDeque.pop_front();
}

void Snake::tryToUseLastStep()
{
    if (lastDirection != Console::Directon::Unknown)
    {
        move(lastDirection);
    }
}
