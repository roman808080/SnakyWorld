#include "Board.h"

#include <iostream>
#include <thread>
#include <string>

#include <stdlib.h>
#include <time.h>

namespace
{
    const Coordinate kDefaultSnakePosition(3, 4);
    const std::chrono::milliseconds kMaxTime(150);
    const std::chrono::milliseconds kMinTime(20);
    const std::chrono::milliseconds kAcceleration(10);

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
    : console(std::make_shared<Console>()),
      score(0),
      pause(kMaxTime)
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

    snake->setSpawn(std::make_shared<Cell>(console, getSpawnCoordinate()));
    snake->setConsumptionObserver(this);

    objects.push_back(snake);
    addScore(0);
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

        std::this_thread::sleep_for(pause);
    }
}

void Board::spawnWasEaten()
{
    snake->setSpawn(std::make_shared<Cell>(console, getSpawnCoordinate()));
    speedUp();
    addScore(1);
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

Coordinate Board::getSpawnCoordinate()
{
    auto spawnCoordinate = getRandomCoordinate(2, sizeX - 1, 1, sizeY - 1);
    bool collided = getInteractionStatus(spawnCoordinate);

    if (collided)
    {
        return getSpawnCoordinate();
    }

    return spawnCoordinate;
}

void Board::speedUp()
{
    if (pause > kMinTime)
    {
        pause -= kAcceleration;
    }
}

void Board::addScore(int gain)
{
    score += gain;
    std::string text("Score: " + std::to_string(score));
    console->printLine(Console::Color::Black, 0, text);
}
