#pragma once

#include <SDL3/SDL.h>

class Colors {
public:
    static constexpr SDL_Color White       = {250, 250, 250, 255};
    static constexpr SDL_Color Black       = {10, 10, 10, 255};
    static constexpr SDL_Color Red         = {255, 0, 0, 255};
    static constexpr SDL_Color Transparent = {0, 0, 0, 0};
    static constexpr SDL_Color Experience  = {0, 240, 32, 255};
    static constexpr SDL_Color Health      = {30, 122, 184, 255};
    static constexpr SDL_Color Asteroid    = {112, 68, 17, 255};
    static constexpr SDL_Color Disabled    = {50, 50, 50, 255};
    static constexpr SDL_Color Lightning   = {240, 251, 62, 255};
};
