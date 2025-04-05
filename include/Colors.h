#pragma once

#include <SDL3/SDL.h>

class Colors {
public:
    static constexpr SDL_Color White       = {255, 255, 255, 255};
    static constexpr SDL_Color Black       = {0, 0, 0, 255};
    static constexpr SDL_Color Red         = {255, 0, 0, 255};
    static constexpr SDL_Color Transparent = {0, 0, 0, 0};
};
