#pragma once

#include <vector>

#include "MenuRow.h"
#include "Board.h"


// represents the main menu.
class Menu
{
public:
    Menu();
    ~Menu();

    // the method in which everything interesting happens.
    void loop();

private:
    // draws itself, espacially usefull after the game when we need to restore the menu.
    void draw();

    // build the whole menu.
    void build();
    // build the part of menu from which we can choose an action.
    int buildOptions(int line);

    // shift option up or down.
    void shiftOption(int shift);
    // update score after the game by calling getter from a border.
    void updateScore(int score);

    // handle direction keys, enter and escape.
    void handleKey(Console::Direction key);
    // runs an operation which is based on the current active option.
    void handleOption();

private:
    std::shared_ptr<Console> console;

    std::shared_ptr<MenuRow> scoreRow; // a reference to score row.
    std::vector<std::shared_ptr<MenuRow>> menuRows; // references to all menu rows.
    std::vector<std::shared_ptr<MenuRow>> optionRows; // options like Play and Exit.

    // the line from which we start our drawing process.
    int startLine;
    // index of an active option.
    int activeOption;
    // if we need to close the game or not.
    bool stop;
};