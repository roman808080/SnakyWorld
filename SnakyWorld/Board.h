#pragma once

#include <memory>
#include <vector>

#include "Cell.h"
#include "Line.h"
#include "Snake.h"

using InteractionObjects = std::vector<std::shared_ptr<ConsoleObject>>;

class Board
{
public:
    Board();
    virtual ~Board();
    void loop();

private:
    bool getInteractionStatus(const Coordinate& snakeHead);

private:
    std::shared_ptr<Console> console;
    std::shared_ptr<Snake> snake;
    InteractionObjects objects;

    int sizeX;
    int sizeY;
};
