#pragma once

#include "Components.h"
#include "Colors.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "GUI.h"
#include "texture.hpp"
#include "utils.hpp"

class LevelUpContainer {
public:
    Texture abilityIcon;
    Texture abilityText;
    Texture abilityContainer;
    uint32_t iconId, textId, containerId;

    LevelUpContainer(
        EntityManager* eManager,
        ShipAbilities ability,
        std::string text,
        FPair pos,
        SDL_Renderer* renderer
    );
};
