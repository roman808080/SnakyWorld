#include "Snake.h"

Snake::Snake(std::shared_ptr<Console> console, const Coordinate& coordinate)
    : console(console), head(coordinate)
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
        --head.first;
        cellDeque.emplace_back(console, head, Console::Color::Yellow);
        return;

    case Console::Directon::Right:
        ++head.first;
        cellDeque.emplace_back(console, head, Console::Color::Yellow);
        return;

    case Console::Directon::Up:
        --head.second;
        cellDeque.emplace_back(console, head, Console::Color::Yellow);
        return;

    case Console::Directon::Down:
        ++head.second;
        cellDeque.emplace_back(console, head, Console::Color::Yellow);
        return;

    case Console::Directon::Unknown:
        // for now just ignore
        return;

    default:
        return;
    }
}

Snake::~Snake()
{
}
