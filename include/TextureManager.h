#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "texture.hpp"

enum class AssetMode { FileSystem, Packed };
inline AssetMode assetMode = AssetMode::Packed;

class TextureManager {
public:
    static TextureManager& instance();
    void init(SDL_Renderer* r);
    bool load(const std::string& id, const std::string& path);
    bool loadFromPack(const std::string& id, const std::string& virtualPath);
    Texture* get(const std::string& id);
    std::vector<Texture*> getAnimationFrames(const std::string& fullPrefix);
    std::vector<Texture*> getAnimationFrames(const std::string& key, const std::string& prefix);
    
    void loadAllFromFolder(const std::string& folder);
    void loadAllFromPack(const std::string& prefix);
    void clear();
    SDL_Renderer* getRenderer() { return renderer; }
    
    private:
    TextureManager() = default;
    SDL_Renderer* renderer = nullptr;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    std::vector<std::string> keys;
};
