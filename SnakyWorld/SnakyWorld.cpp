#include <iostream>
#include "Board.h"

int main()
{
    try
    {
        Board board;
        board.loop();
    }
    catch (const std::exception& exc)
    {
        std::cout << " Something bad happend. Reason = " << exc.what();
    }
    
    return 0;
}
