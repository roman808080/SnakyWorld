#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "Utils.h"

void loop();

int main()
{
    loop();
    return 0;
}

void loop()
{
    Console console;
    int sizeX, sizeY;
    std::tie(sizeX, sizeY) = console.getScreenSize();
    console.drawCeil(1, 1);
}