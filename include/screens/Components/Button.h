#pragma once

#include <stdint.h>
#include <string>

#include "Colors.h"
#include "Fonts.h"
#include "GUIState.h"
#include "texture.hpp"
#include "Box.h"

class EntityManager;
class FPair;
struct SDL_Renderer;

class Button {
public:
    uint32_t id, labelID;
    Texture labelTexture;
    bool hasTexture{ false };
    FPair originalPos, labelPos;
    FPair originalSize, labelSize;
    GUIState lastState = GUIState::Idle;
    std::optional<Box> box;
    Button(EntityManager& em, const std::string& label, FPair pos, FPair size,
        Texture* texture, SDL_Renderer* renderer, uint32_t parent = 0,
        TTF_Font* font = Fonts::Body, SDL_Color color = Colors::White);
    Button(EntityManager& em, const std::string& label, FPair pos, FPair size,
        Texture* texture, const NineGridComponent& nineGrid, SDL_Renderer* renderer, uint32_t parent = 0,
        TTF_Font* font = Fonts::Body, SDL_Color color = Colors::White);
    // ~Button() = default;
    bool wasClicked(EntityManager& em);
    void destroy(EntityManager& em);
    void updateState(EntityManager& em);
private:
    void setSizeFromState(EntityManager& em, uint32_t e, GUIState state);
};
