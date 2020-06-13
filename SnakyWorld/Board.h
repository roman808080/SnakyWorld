#pragma once

#include <memory>
#include <vector>

#include "Cell.h"
#include "Line.h"
#include "Snake.h"

#include "ConsumptionObserver.h"

using InteractionObjects = std::vector<std::shared_ptr<ConsoleObject>>;

class Board: public ConsumptionObserver
{
public:
    Board();
    virtual ~Board();
    void loop();

    virtual void spawnWasEaten() override;

private:
    bool getInteractionStatus(const Coordinate& snakeHead);

private:
    std::shared_ptr<Console> console;
    std::shared_ptr<Snake> snake;
    InteractionObjects objects;

    int sizeX;
    int sizeY;
};
