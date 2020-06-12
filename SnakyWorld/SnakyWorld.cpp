#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "Ceil.h"
#include "Line.h"

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

   Ceil ceil(console, Coordinate(sizeX - 3, sizeY - 3));
   Line topLine(console, Coordinate(0, 0), Coordinate(sizeX - 1, 0));
   Line bottomLine(console, Coordinate(0, sizeY - 1), Coordinate(sizeX, sizeY - 1));

   Line backLine(console, Coordinate(sizeX - 1, 0), Coordinate(sizeX - 1, sizeY - 1));
   Line frontLine(console, Coordinate(0, 1), Coordinate(0, sizeY - 1));
}