#include "Board.h"

#include <iostream>
#include <thread>
#include <string>

#include <random>

namespace
{
    const Coordinate kTopLeftCorner(1, 0);
    const Coordinate kDefaultSnakePosition(3, 4);
    const std::chrono::milliseconds kMaxTime(150);
    const std::chrono::milliseconds kMinTime(20);
    const std::chrono::milliseconds kAcceleration(5);

    int getRandomNumberInRange(int start, int end)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(start, end);

        return dist6(rng);
    }

    Coordinate getRandomCoordinate(int xStart, int xEnd, int yStart, int yEnd)
    {
        return Coordinate(getRandomNumberInRange(xStart, xEnd),
                          getRandomNumberInRange(yStart, yEnd));
    }
}

// there are magic numbers which I didn't clean. Needs to clean up.
Board::Board(std::shared_ptr<Console> console)
    : console(console),
      score(0),
      pause(kMaxTime)
{
    console->drawBackground();

    std::tie(sizeX, sizeY) = console->getScreenSize();

    const Coordinate topLeftCorner(1, 0);
    const Coordinate topRifhtCorner(sizeX - 1, sizeY - 1);

    auto leftLine = std::make_shared<Line>(console, topLeftCorner, Coordinate(sizeX, 0));
    auto rightLine = std::make_shared<Line>(console, Coordinate(1, sizeY - 1), Coordinate(sizeX, sizeY - 1));

    auto bottomLine = std::make_shared<Line>(console, Coordinate(sizeX - 1, 1), topRifhtCorner);
    auto topLine = std::make_shared<Line>(console, Coordinate(1, 1), Coordinate(1, sizeY - 1));

    objects.push_back(leftLine);
    objects.push_back(rightLine);
    objects.push_back(bottomLine);
    objects.push_back(topLine);

    snake = std::make_shared<Snake>(console, kDefaultSnakePosition);

    // setup the snake.
    snake->setSpawn(std::make_shared<Cell>(console, getSpawnCoordinate()));
    snake->setBorders(topLeftCorner, topRifhtCorner);
    snake->setConsumptionObserver(this);

    objects.push_back(snake);
    addScore(0);
}

Board::~Board()
{
}

void Board::loop(bool isDemo)
{
    while (true)
    {
        auto direction = console->getCurrentDirection();
        if (direction == Console::Direction::Esc)
        {
            // returns to menu.
            return;
        }

        if (isDemo)
        {
            // automovent
            snake->move();
        }
        else
        {
            // user based movement.
            snake->move(direction);
        }
        
        auto snakeHead = snake->getCoordindate();
        // checks collisions
        auto interacted = getInteractionStatus(snakeHead);

        // if the snake collided returns to menu. GAME OVER(
        if (interacted)
        {
            return;
        }

        // sleep until next iteration.
        std::this_thread::sleep_for(pause);
    }
}

int Board::getScore()
{
    return score;
}

void Board::spawnWasEaten()
{
    // our snake ate someone. generate a new spawn, speed up the game and add a score.

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
        // generate recursively until no collision occurs.
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
    std::string text(" Score: " + std::to_string(score) + " ");
    console->printLine(Console::Color::Black, 0, text, Console::Color::Red, true);
}
