#include "Board.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

namespace
{
    const Coordinate kDefaultSnakePosition(2, 4);
}


Board::Board()
    : console(std::make_shared<Console>())
{
    console->drawBackground();

    std::tie(sizeX, sizeY) = console->getScreenSize();

    auto leftLine = std::make_shared<Line>(console, Coordinate(0, 0), Coordinate(sizeX, 0));
    auto rightLine = std::make_shared<Line>(console, Coordinate(0, sizeY - 1), Coordinate(sizeX, sizeY - 1));

    auto bottomLine = std::make_shared<Line>(console, Coordinate(sizeX - 1, 1), Coordinate(sizeX - 1, sizeY - 1));
    auto topLine = std::make_shared<Line>(console, Coordinate(0, 1), Coordinate(0, sizeY - 1));

    objects.push_back(leftLine);
    objects.push_back(rightLine);
    objects.push_back(bottomLine);
    objects.push_back(topLine);

    snake = std::make_shared<Snake>(console, kDefaultSnakePosition);
    snake->setSpawn(std::make_shared<Cell>(console, Coordinate(10, 15)));

    objects.push_back(snake);
}

Board::~Board()
{
}

void Board::loop()
{
    while (true)
    {
        auto direction = console->getCurrentDirection();
        if (direction == Console::Directon::Esc)
        {
            return;
        }

        snake->move(direction);
        auto snakeHead = snake->getCoordindate();
        auto interacted = getInteractionStatus(snakeHead);

        if (interacted)
        {
            return;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

bool Board::getInteractionStatus(const Coordinate& snakeHead)
{
    for (const auto& object : objects)
    {
        auto interacted = object->isInteracted(snakeHead);
        if (interacted)
        {
            return true;
        }
    }

    return false;
}
