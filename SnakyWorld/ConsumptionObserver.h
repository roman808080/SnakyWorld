#pragma once
class ConsumptionObserver
{
public:
    virtual void spawnWasEaten() = 0;
    virtual ~ConsumptionObserver() {};
};
