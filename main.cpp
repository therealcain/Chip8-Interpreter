#include "chip8.hpp"

#include <iostream>

int main(int argv, char* argc[]) 
{
    if(argv == 2)
    {
        Chip8 myChip(argc[1]);
        return EXIT_SUCCESS;
    } 
    else 
    {
        std::cout << "Please specify file path!" << std::endl;
    }

    return EXIT_FAILURE;
}