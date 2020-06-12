#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "Console.h"
#include "Ceil.h"

void loop();

int main()
{
    loop();
    return 0;
}

void loop()
{
   auto console = std::make_shared<Console>();

   int sizeX, sizeY;
   std::tie(sizeX, sizeY) = console->getScreenSize();

   Ceil ceil(console, Coordinate(sizeX - 1, sizeY - 1));
}