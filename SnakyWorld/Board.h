#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include "Cell.h"
#include "Line.h"
#include "Snake.h"

#include "ConsumptionObserver.h"

// objects with which the snake can interact.
using InteractionObjects = std::vector<std::shared_ptr<ConsoleObject>>;

// represents whole playable space and the score panel.
class Board: public ConsumptionObserver
{
public:
    Board(std::shared_ptr<Console> console);
    virtual ~Board();

    // in the loop happens main actions. if demo is true than snake plays by itself.
    void loop(bool isDemo = false);

    // after the game is over we need to ask about the current score.
    int getScore();

    // snake signals that it ate by this method (through observer).
    virtual void spawnWasEaten() override;

private:
    // checks if snake is collided with something.
    bool getInteractionStatus(const Coordinate& snakeHead);

    // generate coordinates for a spawn.
    Coordinate getSpawnCoordinate();

    // slightly speeds up the game.
    void speedUp();
    void addScore(int gain);

private:
    std::shared_ptr<Console> console;
    std::shared_ptr<Snake> snake;
    InteractionObjects objects;

    // size of board
    int sizeX;
    int sizeY;

    // keeps the current score
    int score;

    // current sleep time between iterations.
    std::chrono::milliseconds pause;
};
