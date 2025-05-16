#include "TextureManager.h"

#include <filesystem>

TextureManager& TextureManager::instance() {
    static TextureManager instance;
    return instance;
}

void TextureManager::init(SDL_Renderer* r) {
    renderer = r;
    loadAllFromFolder("data/img");
}
bool TextureManager::load(const std::string& id, const std::string& path) {
    if (textures.contains(id)) {
        std::cerr << "Texture IID already exists: " << id << std::endl;
        return false; 
    }

    auto tex = std::make_unique<Texture>();
    tex->m_renderer = renderer;

    if (!tex->loadFromFile(path)) {
        std::cerr << "Failed to load " << path << std::endl;
        textures.erase(id);
        return false;
    }
    textures.emplace(id, std::move(tex));
    std::cout << "Texture loaded, id & path: " << id << " & " << path << std::endl;
    return true;
}
Texture* TextureManager::get(const std::string& id) {
    auto it = textures.find(id);
    if (it != textures.end()) return it->second.get();
    std::cerr << "Texture ID not found: " << id << std::endl;
    return nullptr;
}

void TextureManager::loadAllFromFolder(const std::string& folder) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            std::string id = entry.path().stem().string();
            load(id, path);
        }
    }
}

void TextureManager::clear() {
    textures.clear();
}
