#pragma once

// the class to build a back connection between a board and a snake.
// The snake notifies about changes by this observer.
class ConsumptionObserver
{
public:
    virtual void spawnWasEaten() = 0;
    virtual ~ConsumptionObserver() {};
};
