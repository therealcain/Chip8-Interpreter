#include <SDL2/SDL.h>

#include <iostream>
#include <exception>

#include "chip8.hpp"
#include "window.hpp"

constexpr auto WINDOW_SIZE     = 15;
constexpr auto FRAMERATE_LIMIT = 500;

int main(int argv, char* argc[])
{
    // Debug mode is operating system specific
    #if ENABLE_DEBUG_MODE
        #if (not __linux__ && not _WIN32)
        throw std::runtime_error("Debug mode is not available to your operating system!");
        #endif // not linux && windows
    #endif // ENABLE_DEBUG_MODE
    
    // the second argument is the path to the game
    // the program won't start without a path or too much paths
    if(argv == 2)
    {   
        Window window("Chip8 Emulator", WINDOW_SIZE * CHIP8_WIDTH, WINDOW_SIZE * CHIP8_HEIGHT, CHIP8_WIDTH, CHIP8_HEIGHT);
        Chip8 chip(argc[1], window);

        Uint32 start_fps;
        while(window.is_running())
        {
            start_fps = SDL_GetTicks();

            chip.cycle();
            window.event_handler(chip.keypads);
            window.update(chip.display);

            // FPS Cap
            if(1000 / FRAMERATE_LIMIT > SDL_GetTicks() - start_fps)
            {
                SDL_Delay(1000 / FRAMERATE_LIMIT - (SDL_GetTicks() - start_fps));
            }
        }

        return EXIT_SUCCESS;
    }
    else
    {
        std::cerr << "Please specify a correct path!\n";
    }
    

    return EXIT_FAILURE;
}
