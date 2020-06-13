#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include "Cell.h"
#include "Line.h"
#include "Snake.h"

#include "ConsumptionObserver.h"

using InteractionObjects = std::vector<std::shared_ptr<ConsoleObject>>;

class Board: public ConsumptionObserver
{
public:
    Board(std::shared_ptr<Console> console);
    virtual ~Board();

    void loop();
    int getScore();

    virtual void spawnWasEaten() override;

private:
    bool getInteractionStatus(const Coordinate& snakeHead);
    Coordinate getSpawnCoordinate();
    void speedUp();
    void addScore(int gain);

private:
    std::shared_ptr<Console> console;
    std::shared_ptr<Snake> snake;
    InteractionObjects objects;

    int sizeX;
    int sizeY;

    int score;

    std::chrono::milliseconds pause;
};
