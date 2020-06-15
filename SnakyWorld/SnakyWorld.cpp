#include <iostream>
#include "Menu.h"

int main()
{
    try
    {
        Menu menu;
        menu.loop();
    }
    catch (const std::exception& exc)
    {
        std::cout << " Something bad happend. Reason = " << exc.what();
    }
    
    return 0;
}
