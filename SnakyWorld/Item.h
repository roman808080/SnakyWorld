#pragma once
#include <memory>

#include "Console.h"

class Item
{
public:
    Item(std::shared_ptr<Console> console,
         const std::string& name, int line);
    ~Item();

    void setActivity(bool isActive);
    void draw();

private:
    std::shared_ptr<Console> console;

    std::string nonActive;
    std::string active;

    int line;
};
