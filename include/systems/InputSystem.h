#pragma once

#include "ClickHandler.h"
#include "System.h"

#include <SDL3/SDL.h>

class InputSystem : public System {
public:
    void update(EntityManager& eMgr, const double& dT) override;
    FPair adjustMousePos(FPair mousePos);
private:
    SDL_Event e;
    ClickHandler clickHandler;
};
