#include "chip8.hpp"

#include <iostream>

int main(int argv, char* argc[]) 
{
    // the second argument is the path to the game
    // the program won't start without a path or too much paths
    if(argv == 2)
    {
        Chip8 myChip(argc[1]);
        return EXIT_SUCCESS;
    } 
    else 
    {
        std::cout << "Please specify the correct file path!" << std::endl;
    }

    return EXIT_FAILURE;
}