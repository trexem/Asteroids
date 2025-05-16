#pragma once

#include <stdint.h>
#include <string>

#include "Colors.h"
#include "Fonts.h"
#include "GUIState.h"
#include "texture.hpp"

class EntityManager;
class FPair;
class SDL_Renderer;

class Label {
public: 
    uint32_t id;
    Texture texture;
    bool interactive;
    FPair originalPos, originalSize;
    GUIState lastState {GUIState::Idle};
    TTF_Font* font;
    SDL_Color color;
    Label(EntityManager& em, const std::string& label, FPair pos, FPair size,
        SDL_Renderer* renderer, uint32_t parent = 0, TTF_Font* font = Fonts::Body, 
        SDL_Color color = Colors::White, bool interactive = false);
    void destroy(EntityManager& em);
    void setText(const std::string& t);
private:
    std::string text;
};
