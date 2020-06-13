#include "Item.h"

namespace
{
    std::string wrapText(const std::string& text, bool isActive)
    {
        std::string newText(text);

        if (isActive)
        {
            //
        }
        return std::string();
    }

}


Item::Item(std::shared_ptr<Console> console,
           const std::string& name, int line)
{
}

void Item::setActivity(bool isActive)
{
}

void Item::draw()
{
}

Item::~Item()
{
}
