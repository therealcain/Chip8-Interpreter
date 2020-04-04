#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

#include <cstdint>
#include <string>

class Window
{
public:
    Window(const std::string& str, int width, int height);
    ~Window();

    bool run() noexcept;
    void clear() noexcept;
    void render() noexcept;

private:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture*  texture  = nullptr;

    // let chip8 access all of window members
    friend class Chip8;
};

#endif // WINDOW_HPP