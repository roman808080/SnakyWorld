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
    const std::vector<std::string> kMenuOptions {"Play", "Exit"};
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
    console->drawBackground();
    for (auto& row : menuRows)
    {
        row->draw();
    }

    optionRows[activeOption]->draw(true);
}

void Menu::build()
{
    int currentLine = startLine;

    for (const auto& rowText: kMenuTemplate)
    {
        if (rowText == "<<score>>")
        {
            scoreRow = std::make_shared<MenuRow>(console, "", currentLine);
            menuRows.push_back(scoreRow);
            updateScore(0);

            ++currentLine;
        }
        else if (rowText == "<<options>>")
        {
            currentLine = buildOptions(currentLine);
        }
        else
        {
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

    std::string scoreString = "Current score: " + std::to_string(score);
    scoreRow->setText(scoreString);
    scoreRow->draw();
}

void Menu::handleKey(Console::Directon key)
{
    switch (key)
    {
    case Console::Directon::Esc:
        stop = true;
        return;
    case Console::Directon::Enter:
        handleOption();
        return;
    case Console::Directon::Up:
        shiftOption(-1);
        return;
    case Console::Directon::Down:
        shiftOption(1);
    default:
        return;
    }
}

void Menu::handleOption()
{
    auto action = optionRows[activeOption]->getText();
    if (action == "Play")
    {
        auto board = std::make_shared<Board>(console);
        board->loop();

        draw();
        updateScore(board->getScore());
    }
    else if (action == "Exit")
    {
        stop = true;
    }
}
