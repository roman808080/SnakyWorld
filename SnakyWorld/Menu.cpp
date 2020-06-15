#include "Menu.h"
#include <thread>

namespace
{
    // the pause between iterations.
    const std::chrono::milliseconds kPause(200);

    // brief template how the menu will look like.
    const std::vector<std::string> kMenuTemplate { "", "",
                                                   "SNAKY WORLD",
                                                   "",
                                                   "------------------",
                                                   "<<score>>",
                                                   "------------------",
                                                   "",
                                                   "<<options>>",
                                                   "", "" };

    // Prerecorded options. For now, there are no needs for dynamic menu.
    const std::vector<std::string> kMenuOptions {"Play", "Demo", "Exit"};
}


Menu::Menu()
    : console(std::make_shared<Console>()),
      startLine(0),
      activeOption(0),
      stop(true)
{
    int xSize, _;
    std::tie(xSize, _) = console->getScreenSize();

    startLine = xSize / 3;

    build();
    draw();
}

Menu::~Menu()
{
    // clear after ourselves
    console->clear();
}

void Menu::loop()
{
    stop = false;
    while (!stop)
    {
        std::this_thread::sleep_for(kPause);
        auto direction = console->getCurrentDirection();
        handleKey(direction);
    }
}

void Menu::draw()
{
    // redraws step by step everything.
    console->drawBackground();
    for (auto& row : menuRows)
    {
        row->draw();
    }

    // draws the active element.
    optionRows[activeOption]->draw(true);
}

void Menu::build()
{
    int currentLine = startLine;

    for (const auto& rowText: kMenuTemplate)
    {
        if (rowText == "<<score>>")
        {
            // we need to remember where score is. Save it in an additional variable.
            scoreRow = std::make_shared<MenuRow>(console, "", currentLine);
            menuRows.push_back(scoreRow);
            updateScore(0);

            // increase the current line index because we used the previous one already.
            ++currentLine;
        }
        else if (rowText == "<<options>>")
        {
            // generate options in the separeted method
            currentLine = buildOptions(currentLine);
        }
        else
        {
            // if it is just a row lets put it in a common backet and forget about this one.
            auto menuRow = std::make_shared<MenuRow>(console, rowText, currentLine);
            menuRows.push_back(menuRow);
            ++currentLine;
        }
        
    }
}

int Menu::buildOptions(int line)
{
    for (const auto& rowText : kMenuOptions)
    {
        auto menuRow = std::make_shared<MenuRow>(console, rowText, line);
        menuRows.push_back(menuRow);
        optionRows.push_back(menuRow);

        ++line;
    }
    return line;
}

void Menu::shiftOption(int shift)
{
    // reset last option to non-active state.
    optionRows[activeOption]->draw();

    // calculate and draw a new row.
    activeOption = (optionRows.size() + activeOption + shift) % optionRows.size();
    optionRows[activeOption]->draw(true);
}

void Menu::updateScore(int score)
{
    if (!scoreRow)
    {
        return;
    }

    // generate a new score string.
    std::string scoreString = "Current score: " + std::to_string(score);
    scoreRow->setText(scoreString);
    scoreRow->draw();
}

void Menu::handleKey(Console::Direction key)
{
    switch (key)
    {
    case Console::Direction::Esc: // quits the game.
        stop = true;
        return;
    case Console::Direction::Enter: // triggers an action.
        handleOption();
        return;
    case Console::Direction::Up: // choose the next option above
        shiftOption(-1);
        return;
    case Console::Direction::Down: // choose the next option down
        shiftOption(1);
    default:
        return; // or just ignore the key.
    }
}

void Menu::handleOption()
{
    auto action = optionRows[activeOption]->getText();

    // decides what to do based on the screen text.
    // Can be buggy in the future, maybe refactoring is needed.
    if (action == "Play" or action == "Demo")
    {
        auto board = std::make_shared<Board>(console);
        board->loop(action == "Demo"); // starts demo if the action is equal to "Demo"

        auto currentScore = board->getScore();
        board.reset(); // destroy the board before redraw the menu to clean the screen.

        draw();
        updateScore(currentScore);
    }
    else if (action == "Exit")
    {
        stop = true;
    }
}
