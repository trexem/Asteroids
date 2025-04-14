#pragma once

#include <SDL3/SDL.h>

class Colors {
public:
    static constexpr SDL_Color White       = {250, 250, 250, 255};
    static constexpr SDL_Color Black       = {10, 10, 10, 255};
    static constexpr SDL_Color Red         = {255, 0, 0, 255};
    static constexpr SDL_Color Transparent = {0, 0, 0, 0};
    static constexpr SDL_Color Experience  = {0, 240, 32, 255};
};
