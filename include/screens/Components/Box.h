#pragma once

#include <stdint.h>

#include "Colors.h"
#include "GUIState.h"
#include "texture.hpp"
#include "Components.h"

class EntityManager;
class FPair;

class Box {
public:
    uint32_t id;
    FPair originalPos, originalSize;
    GUIState lastState = GUIState::Idle;

    Box(EntityManager& em, FPair pos, FPair size, Texture* texture,
        bool use9Grid = false, NineGridComponent nineGrid = {},
        uint32_t parent = 0, SDL_Color color = Colors::White);

    void updateState(EntityManager& eM);
    void destroy(EntityManager& eM);

private:
    void setSizeFromState(EntityManager& em, GUIState state);
};
