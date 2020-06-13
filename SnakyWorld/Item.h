#pragma once
#include <memory>

#include "Console.h"

class Item
{
public:
    Item(std::shared_ptr<Console> console,
        const std::string& text,
        int line);
    ~Item();

    void setAccessibility(bool accessible);
    bool isAccessible();
    void draw(bool isActive);

    void setText(const std::string& text);
    std::string getText();

private:
    std::shared_ptr<Console> console;
    std::string text;

    int line;
    bool accessible;
};
