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
    void drawAll();
    void createItems();
    void moveActiveItem(bool forward);

private:
    std::shared_ptr<Console> console;
    std::vector<Item> menuItems;

    int startLine;
    int activeItem;
};