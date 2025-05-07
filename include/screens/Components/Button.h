#pragma once

#include <stdint.h>
#include <string>

#include "texture.hpp"

class EntityManager;
class FPair;
class SDL_Renderer;

class Button {
public:
    uint32_t id, labelID;
    Texture labelTexture;
    Button(EntityManager* em, const std::string& label, FPair pos, FPair size,
        Texture* texture, SDL_Renderer* renderer, uint32_t parent = 0);
    // ~Button() = default;
    bool wasClicked(EntityManager* em);
    void destroy(EntityManager* em);
private:
};
