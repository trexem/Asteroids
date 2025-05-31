#pragma once

#include "Components.h"
#include "Colors.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "GUI.h"
#include "texture.hpp"
#include "utils.hpp"
#include "screens/Components/Label.h"

class LevelUpContainer {
public:
    Texture* abilityIcon;
    Texture abilityText;
    Texture abilityContainer;
    std::unique_ptr<Label> title;
    std::unique_ptr<Label> description;
    uint32_t iconId, textId, containerId;

    LevelUpContainer(
        EntityManager& eManager,
        AbilityChoice choice,
        std::string text,
        FPair pos,
        SDL_Renderer* renderer
    );
    void destroy(EntityManager& eM);
};
