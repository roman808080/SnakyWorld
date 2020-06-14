#pragma once
#include <memory>

#include "Console.h"

class MenuRow
{
public:
    MenuRow(std::shared_ptr<Console> console,
            const std::string& text,
            int line);

    void draw(bool isActive = false);

    void setText(const std::string& text);
    std::string getText();

private:
    std::shared_ptr<Console> console;
    std::string text;

    int line;
};
