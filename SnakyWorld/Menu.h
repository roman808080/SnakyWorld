#pragma once

#include <vector>

#include "Item.h"

class Menu
{
public:
    Menu();
    ~Menu();

private:
    std::vector<Item> menuItems;
};