#pragma once
#ifndef __INPUT_SYSTEM_H_
#define __INPUT_SYSTEM_H_

#include "System.h"
#include "FPair.h"
#include "ClickMessage.h"
#include "ClickHandler.h"
#include "GameStateManager.h"
#include "KeyboardMessage.h"
#include "MouseMotionMessage.h"
#include "MessageManager.h"

#include <SDL3/SDL.h>

class InputSystem : public System {
public:
    void update();
private:
    SDL_Event e;
    ClickHandler clickHandler;
};

#endif // !__INPUT_SYSTEM_H_
