#include "MenuRow.h"

namespace
{
    const unsigned int wrapperSize = 30;
    const unsigned int activeShift = 4;

    // wraps text. the function possibly needs the refactoring.
    // there is possibly a duplication with functions in the Console class.
    std::string wrapText(const std::string& text, bool isActive)
    {
        unsigned int wrapUntil = wrapperSize;
        std::string newText(text);
        if (isActive)
        {
            // wraps with additional elements.
            newText = "<" + text + ">";
            wrapUntil += activeShift;
        }

        bool right = true;
        while (newText.size() < wrapUntil)
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

MenuRow::MenuRow(std::shared_ptr<Console> console,
           const std::string& text,
           int line)
    : console(console),
      text(text),
      line(line)
{
    draw(false);
}

void MenuRow::draw(bool isActive)
{
    std::string menuText(wrapText(text, isActive));
    auto rowBackground = Console::Color::Red;
    if (isActive)
    {
        rowBackground = Console::Color::ActiveBackground;
    }

    console->printLine(Console::Color::Background, line, menuText, rowBackground, true);
}

void MenuRow::setText(const std::string& text)
{
    this->text = text;
}

std::string MenuRow::getText()
{
    return text;
}
