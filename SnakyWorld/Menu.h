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
    void draw();

    void build();
    int buildOptions(int line);

    void shiftOption(int shift);
    void updateScore(int score);

private:
    std::shared_ptr<Console> console;

    std::shared_ptr<MenuRow> scoreRow; // a reference to score row.
    std::vector<std::shared_ptr<MenuRow>> menuRows; // references to all menu rows.
    std::vector<std::shared_ptr<MenuRow>> optionRows; // options like Play and Exit.

    int startLine;
    int activeOption;
};