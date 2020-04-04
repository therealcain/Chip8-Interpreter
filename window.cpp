#include "window.hpp"

#include <exception>
#include <iostream>

Window::Window(const std::string& str, int width, int height)
{
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        const std::string err = "SDL could not initialize! SDL_Error: " + *SDL_GetError();
        throw std::runtime_error(err);
    }

    // Create SDL window
    window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if(window == nullptr)
    {
        const std::string err = "Window could not be created! SDL_Error: " + *SDL_GetError();
        throw std::runtime_error(err);
    }
    
    // Create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

bool Window::run() noexcept
{
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0)
    {
        if(event.type == SDL_QUIT) 
        {
            return false;
        }
    }

    return true;
}

void Window::clear() noexcept
{
    // clear the pipeline
    SDL_RenderClear(renderer);
}

void Window::render() noexcept
{
    // render chip8
    SDL_RenderPresent(renderer);
}

Window::~Window()
{
    // free all SDL memory
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}