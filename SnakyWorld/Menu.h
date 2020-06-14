#pragma once

#include <vector>

#include "MenuRow.h"
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
    std::vector<MenuRow> menuItems;

    int startLine;
    int activeItem;
};