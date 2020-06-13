#include "Menu.h"

namespace
{
    const int kMaxMenuItemsSize = 10;
}


Menu::Menu()
    : console(std::make_shared<Console>()),
      startLine(0),
      activeItem(0)
{
    console->drawBackground();

    int xSize, _;
    std::tie(xSize, _) = console->getScreenSize();

    startLine = xSize / 3;
    createItems();
}

Menu::~Menu()
{
    console->clear();
}

void Menu::loop()
{
    auto board = std::make_shared<Board>(console);
    board->loop();
}

void Menu::createItems()
{
    std::vector<std::string> labels{"", "",
                                    "SNAKY WORLD",
                                    "----------------",
                                    "|Current Score: 0|",
                                    "----------------",
                                    "Play",
                                    "Exit",
                                    "", ""};
    int currentLine = startLine;
    menuItems.reserve(kMaxMenuItemsSize);

    for (const auto& label: labels)
    {
        menuItems.emplace_back(console, label, currentLine);
        ++currentLine;
    }

    for (int i = 0; i < menuItems.size(); ++i)
    {
        if (menuItems[i].getText() == "Play" or
            menuItems[i].getText() == "Exit")
        {
            menuItems[i].setAccessibility(true);
        }
    }
}
