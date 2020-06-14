#include "Menu.h"
#include <thread>

namespace
{
    const std::chrono::milliseconds kPause(200);

    const std::vector<std::string> kMenuTemplate { "", "",
                                                   "SNAKY WORLD",
                                                   "",
                                                   "----------------",
                                                   "<score>",
                                                   "----------------",
                                                   "",
                                                   "<menu>",
                                                   "", "" };
}


Menu::Menu()
    : console(std::make_shared<Console>()),
      startLine(0),
      activeItem(0)
{
    int xSize, _;
    std::tie(xSize, _) = console->getScreenSize();

    startLine = xSize / 3;
    createItems();
    moveActiveItem(true);
    drawAll();
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
            moveActiveItem(false);
        }

        if (direction == Console::Directon::Down)
        {
            moveActiveItem(true);
        }

        if (false)
        {
            auto board = std::make_shared<Board>(console);
            board->loop();
        }

        std::this_thread::sleep_for(kPause);
    }
}

void Menu::drawAll()
{
    console->drawBackground();
    for (auto& item : menuItems)
    {
        item.draw(false);
    }
    menuItems[activeItem].draw(true);
}

void Menu::createItems()
{
    std::vector<std::string> labels{"", "",
                                    "SNAKY WORLD",
                                    "",
                                    "----------------",
                                    "Current Score: 0",
                                    "----------------",
                                    "",
                                    "Play",
                                    "Exit",
                                    "", ""};
    int currentLine = startLine;
    menuItems.reserve(labels.size());

    for (const auto& label: labels)
    {
        menuItems.emplace_back(console, label, currentLine);
        ++currentLine;
    }

    for (size_t i = 0; i < menuItems.size(); ++i)
    {
        if (menuItems[i].getText() == "Play" or
            menuItems[i].getText() == "Exit")
        {
            menuItems[i].setAccessibility(true);
        }
    }
}

void Menu::moveActiveItem(bool forward)
{
    int direction = 0;
    if (forward)
    {
        direction = 1;
    }
    else
    {
        direction = -1;
    }

    menuItems[activeItem % menuItems.size()].draw(false);
    while (true)
    {
        activeItem = (menuItems.size() + activeItem + direction) % menuItems.size();
        if (menuItems[activeItem].isAccessible())
        {
            menuItems[activeItem].draw(true);
            return;
        }
    }
}
