#pragma once

#include "ClickHandler.h"
#include "System.h"

#include <SDL3/SDL.h>

class InputSystem : public System {
public:
    void update();
private:
    SDL_Event e;
    ClickHandler clickHandler;
};
