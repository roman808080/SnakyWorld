#include "Board.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include <stdlib.h>
#include <time.h>

namespace
{
    const Coordinate kDefaultSnakePosition(3, 4);

    int getRandomNumberInRange(int start, int end)
    {
        srand(time(nullptr));
        return rand() % (end - start) + start;
    }

    Coordinate getRandomCoordinate(int xStart, int xEnd, int yStart, int yEnd)
    {
        return Coordinate(getRandomNumberInRange(xStart, xEnd),
                          getRandomNumberInRange(yStart, yEnd));
    }
}

Board::Board()
    : console(std::make_shared<Console>())
{
    console->drawBackground();

    std::tie(sizeX, sizeY) = console->getScreenSize();

    auto leftLine = std::make_shared<Line>(console, Coordinate(1, 0), Coordinate(sizeX, 0));
    auto rightLine = std::make_shared<Line>(console, Coordinate(1, sizeY - 1), Coordinate(sizeX, sizeY - 1));

    auto bottomLine = std::make_shared<Line>(console, Coordinate(sizeX - 1, 1), Coordinate(sizeX - 1, sizeY - 1));
    auto topLine = std::make_shared<Line>(console, Coordinate(1, 1), Coordinate(1, sizeY - 1));

    objects.push_back(leftLine);
    objects.push_back(rightLine);
    objects.push_back(bottomLine);
    objects.push_back(topLine);

    snake = std::make_shared<Snake>(console, kDefaultSnakePosition);

    auto spawnCoordinate = getRandomCoordinate(2, sizeX - 1, 1, sizeY - 1);
    snake->setSpawn(std::make_shared<Cell>(console, spawnCoordinate));
    snake->setConsumptionObserver(this);

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

void Board::spawnWasEaten()
{
    auto spawnCoordinate = getRandomCoordinate(2, sizeX - 1, 1, sizeY - 1);
    snake->setSpawn(std::make_shared<Cell>(console, spawnCoordinate));
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
