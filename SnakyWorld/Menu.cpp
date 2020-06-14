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
                                                   "----------------",
                                                   "<<score>>",
                                                   "----------------",
                                                   "",
                                                   "<<options>>",
                                                   "", "" };

    // Prerecorded options. For now, there are no needs for dynamic menu.
    const std::vector<std::string> kMenuOptions {"Play", "Exit"};

    const unsigned int kGapsAmount = 2; // <score> and <menu>
    const unsigned int kScoreSize = 1; // just to avoid a magic number
}


Menu::Menu()
    : console(std::make_shared<Console>()),
      startLine(0),
      activeOption(0)
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
    while (true)
    {
        auto direction = console->getCurrentDirection();
        if (direction == Console::Directon::Esc)
        {
            return;
        }

        if (direction == Console::Directon::Up)
        {
            shiftOption(-1);
        }

        if (direction == Console::Directon::Down)
        {
            shiftOption(1);
        }

        if (false)
        {
            auto board = std::make_shared<Board>(console);
            board->loop();
        }

        std::this_thread::sleep_for(kPause);
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

    int menuSize = kMenuTemplate.size() + kMenuOptions.size() + kScoreSize - kGapsAmount;
    menuRows.reserve(menuSize);

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
