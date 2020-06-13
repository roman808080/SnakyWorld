#include "Item.h"

namespace
{
    const int wrapperSize = 30;

    std::string wrapText(const std::string& text, bool isActive)
    {
        std::string newText(text);
        if (isActive)
        {
            newText = "<" + text + ">";
        }

        bool right = true;
        while (newText.size() < wrapperSize)
        {
            if (right)
            {
                newText += " ";
                right = false;
            }
            else
            {
                newText = " " + newText;
                right = true;
            }
        }

        return newText;
    }

}

Item::Item(std::shared_ptr<Console> console,
           const std::string& text,
           int line)
    : console(console),
      text(text),
      line(line),
      accessible(false)
{
    draw(false);
}

void Item::setAccessibility(bool accessible)
{
    this->accessible = accessible;
}

bool Item::isAccessible()
{
    return accessible;
}

void Item::draw(bool isActive)
{
    std::string menuText(wrapText(text, isActive));
    console->printLine(Console::Color::Background, line, menuText, Console::Color::Red, true);
}

void Item::setText(const std::string& text)
{
    this->text = text;
}

std::string Item::getText()
{
    return text;
}

Item::~Item()
{
    console->printLine(Console::Color::Background, line, "", Console::Color::Background, false);
}
