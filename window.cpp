#include "window.hpp"

#include <exception>
#include <iostream>
#include <map>

Window::Window(const std::string& str, int width, int height, int chip_width, int chip_height)
    : m_chip_width(chip_width)
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
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, chip_width, chip_height);
}

void Window::event_handler(std::array<uint8_t, 16>& keypads) noexcept
{
    // which index the key is
    static std::map<uint8_t, SDL_Keycode> Keys {
        { 0,   SDLK_x }, { 1,  SDLK_1  }, { 2,   SDLK_2  },
        { 3,   SDLK_3 }, { 4,  SDLK_q  }, { 5,   SDLK_w  },
        { 6,   SDLK_e }, { 7,  SDLK_a  }, { 8,   SDLK_s  },
        { 9,   SDLK_d }, { 0xA, SDLK_z } ,{ 0xB, SDLK_c  },
        { 0xC, SDLK_4 }, { 0xD, SDLK_r }, { 0xE, SDLK_f  },
        { 0xF, SDLK_v }
    };

    SDL_Event event;
    while(SDL_PollEvent(&event) != 0)
    {
        if(event.type == SDL_QUIT) 
            running = false;
        
        for(auto[index, key] : Keys)
        {
            if(event.key.keysym.sym == key)
            {   
                if(event.type == SDL_KEYDOWN)
                    keypads[index] = 1;
                 else if(event.type == SDL_KEYUP)
                    keypads[index] = 0;
            }
        }
    }
}

void Window::update(std::array<uint32_t, 2048>& display) noexcept
{
    // clear the the render and assign the texture 
    SDL_UpdateTexture(texture, nullptr, display.data(), sizeof(decltype(display[0])) * m_chip_width);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

Window::~Window()
{
    // free all SDL memory
    SDL_DestroyTexture(texture);
    texture = nullptr;

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}