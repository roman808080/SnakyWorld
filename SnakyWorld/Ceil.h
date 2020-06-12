#pragma once
#include <memory>

#include "ConsoleObject.h"
#include "Console.h"

class Ceil: public ConsoleObject
{
public:
    Ceil(std::shared_ptr<Console> console, const Coordinate& coordinate);

    virtual Interaction isInteracted(const ConsoleObject& consoleObject) override;
    virtual Coordinate getCoordindate() override;

    virtual ~Ceil();

private:
    Coordinate coordinate;
    std::shared_ptr<Console> console;
};

