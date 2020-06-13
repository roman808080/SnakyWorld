#pragma once

#include <vector>

#include "Item.h"
#include "Board.h"

class Menu
{
public:
    Menu();
    ~Menu();

    void loop();

private:
    void createItems();

private:
    std::shared_ptr<Console> console;

    std::vector<Item> menuItems;

    int startLine;
    int activeItem;
};