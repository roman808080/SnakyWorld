#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

#include "Cell.h"
#include "Line.h"
#include "Snake.h"

using InteractionObjects = std::vector<std::shared_ptr<ConsoleObject>>;

void loop();
bool getInteractionStatus(const InteractionObjects& objects, const Coordinate& snakeHead);

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
   console->drawBackground();

   int sizeX, sizeY;
   std::tie(sizeX, sizeY) = console->getScreenSize();

   InteractionObjects objects;

   auto leftLine = std::make_shared<Line>(console, Coordinate(0, 0), Coordinate(sizeX - 1, 0));
   auto rightLine = std::make_shared<Line>(console, Coordinate(0, sizeY - 1), Coordinate(sizeX - 1, sizeY - 1));
   auto bottomLine = std::make_shared<Line>(console, Coordinate(sizeX - 1, 0), Coordinate(sizeX - 1, sizeY - 1));
   auto topLine = std::make_shared<Line>(console, Coordinate(0, 1), Coordinate(0, sizeY - 1));

   objects.push_back(leftLine);
   objects.push_back(rightLine);
   objects.push_back(bottomLine);
   objects.push_back(topLine);

   auto snake = std::make_shared<Snake>(console, Coordinate(5, 5));
   snake->setSpawn(std::make_shared<Cell>(console, Coordinate(10, 15)));
   objects.push_back(snake);

   while (true)
   {
       auto direction = console->getCurrentDirection();
       if (direction == Console::Directon::Esc)
       {
           return;
       }

       snake->move(direction);
       auto snakeHead = snake->getCoordindate();
       auto interacted = getInteractionStatus(objects, snakeHead);

       if (interacted)
       {
           return;
       }

       std::this_thread::sleep_for(std::chrono::milliseconds(300));
   }
}

bool getInteractionStatus(const InteractionObjects& objects, const Coordinate& snakeHead)
{
    for (const auto& object: objects)
    {
        auto interacted = object->isInteracted(snakeHead);
        if (interacted)
        {
            return true;
        }
    }

    return false;
}