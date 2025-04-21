#pragma once

#include <SDL3/SDL.h>

#include "message.h"

class KeyboardMessage : public Message {
public:
    SDL_Keycode key;
    bool pressed;
    KeyboardMessage(SDL_Keycode key, bool pressed) : key(key), pressed(pressed) {}
};
