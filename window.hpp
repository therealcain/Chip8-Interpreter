#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>

#include <cstdint>
#include <string>
#include <array>

class Window
{
public:
    Window(const std::string& str, int width, int height, int chip_width, int chip_height);
    ~Window();

    void event_handler(std::array<uint8_t, 16>& keypads) noexcept;
    void update(std::array<uint32_t, 2048>& display) noexcept;

    constexpr bool is_running() noexcept
    {
        return running;
    }

private:
    SDL_Window*    window   = nullptr;
    SDL_Renderer*  renderer = nullptr;
    SDL_Texture*   texture  = nullptr;

    int m_chip_width;

    bool running = true;

    // let chip8 access all of window members
    friend class Chip8;
};

#endif // WINDOW_HPP