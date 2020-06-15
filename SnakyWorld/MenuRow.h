#pragma once
#include <memory>

#include "Console.h"

// primitive of menu. represents or option or just some visible object.
class MenuRow
{
public:
    MenuRow(std::shared_ptr<Console> console,
            const std::string& text,
            int line);

    // draws itself. if active draws active state.
    void draw(bool isActive = false);

    // changes the text of row.
    void setText(const std::string& text);
    // returns current text.
    std::string getText();

private:
    std::shared_ptr<Console> console;
    std::string text;

    // the line at which the row is.
    int line;
};
