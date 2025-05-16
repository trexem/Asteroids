#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "texture.hpp"

class TextureManager {
public:
    static TextureManager& instance();
    void init(SDL_Renderer* r);
    bool load(const std::string& id, const std::string& path);
    Texture* get(const std::string& id);

    void loadAllFromFolder(const std::string& folder);
    void clear();

private:
    TextureManager() = default;
    SDL_Renderer* renderer = nullptr;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
};
