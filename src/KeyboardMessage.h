#pragma once
#ifndef __KEYBOARD_MESSAGE_H_
#define __KEYBOARD_MESSAGE_H_

#include "message.h"
#include <SDL3/SDL.h>

class KeyboardMessage : public Message {
public:
    SDL_Keycode key;
    bool pressed;
    KeyboardMessage(SDL_Keycode key, bool pressed) : key(key), pressed(pressed) {}
};

#endif //!__KEYBOARD_MESSAGE_H_
