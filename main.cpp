#include <SDL2/SDL.h>

#include <iostream>
#include <exception>

#include "chip8.hpp"
#include "window.hpp"

#if ENABLE_DEBUG_MODE
    #if (not __linux__ && not _WIN32)
    throw std::runtime_error("Debug mode is not available to your operating system!");
    #endif // not linux && windows
#endif // ENABLE_DEBUG_MODE

constexpr auto WINDOW_SIZE = 15;

int main(int argv, char* argc[])
{
    // the second argument is the path to the game
    // the program won't start without a path or too much paths
    if(argv == 2)
    {   
        Window window("Chip8 Emulator", WINDOW_SIZE * CHIP8_WIDTH, WINDOW_SIZE * CHIP8_HEIGHT);
        Chip8 chip(argc[1], window);

        while(window.run())
        {
            chip.cycle();
        }

        return EXIT_SUCCESS;
    }
    else
    {
        std::cerr << "Please specify a correct path!\n";
    }
    

    return EXIT_FAILURE;
}