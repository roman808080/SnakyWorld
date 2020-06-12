#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "Cell.h"
#include "Line.h"
#include "Snake.h"

void loop();

int main()
{
    try
    {
        loop();
    }
    catch (const std::exception& exc)
    {
        std::cout << " Something bad happend. Reason = " << exc.what();
    }
    
    return 0;
}

void loop()
{
   auto console = std::make_shared<Console>();

   int sizeX, sizeY;
   std::tie(sizeX, sizeY) = console->getScreenSize();

   Cell cell(console, Coordinate(sizeX - 3, sizeY - 3));
   Line leftLine(console, Coordinate(0, 0), Coordinate(sizeX - 1, 0));
   Line rightLine(console, Coordinate(0, sizeY - 1), Coordinate(sizeX - 1, sizeY - 1));

   Line bottomLine(console, Coordinate(sizeX - 1, 0), Coordinate(sizeX - 1, sizeY));
   Line topLine(console, Coordinate(0, 1), Coordinate(0, sizeY - 1));

   Snake snake(console, Coordinate(5, 5));
   while (true)
   {
       auto direction = console->getCurrentDirection();
       if (direction == Console::Directon::Esc)
       {
           return;
       }

       snake.move(direction);
       std::this_thread::sleep_for(std::chrono::seconds(1));
   }
}