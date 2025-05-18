#include "TextureManager.h"
#include "PackReader.h"
#include "utils.hpp"

#include <filesystem>

TextureManager& TextureManager::instance() {
    static TextureManager instance;
    return instance;
}

void TextureManager::init(SDL_Renderer* r) {
    renderer = r;
    if (assetMode == AssetMode::FileSystem) {
        loadAllFromFolder("data/img");
    } else {
        loadAllFromPack("img/");
    }
}

bool TextureManager::load(const std::string& id, const std::string& path) {
    std::string p = normalizePath(path);
    if (textures.contains(id)) {
        std::cerr << "Texture ID already exists: " << id << std::endl;
        return false; 
    }

    auto tex = std::make_unique<Texture>();
    tex->m_renderer = renderer;

    if (!tex->loadFromFile(p)) {
        std::cerr << "Failed to load " << p << std::endl;
        textures.erase(id);
        return false;
    }
    textures.emplace(id, std::move(tex));
    std::cout << "Texture loaded, id & path: " << id << " & " << p << std::endl;
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
            std::string path = normalizePath(entry.path().string());
            std::string id = normalizePath(entry.path().stem().string());
            load(id, path);
        }
    }
}

void TextureManager::clear() {
    textures.clear();
}

void TextureManager::loadAllFromPack(const std::string& prefix) {
    auto files = PackReader::instance().listFiles(prefix);
    for (const auto& path : files) {
        std::string id = normalizePath(path.substr(prefix.length())); // Remove "img/" to use as id
        //Remove extension as well
        size_t dotPos = id.find_last_of('.');
        if (dotPos != std::string::npos) {
            id = id.substr(0, dotPos);
        }
        loadFromPack(id, path);
    }
}

bool TextureManager::loadFromPack(const std::string& id, const std::string& virtualPath) {
    std::string path = normalizePath(virtualPath);
    if (textures.contains(id)) {
        std::cerr << "Texture ID already exists (from pack): " << id << std::endl;
        return false; 
    }

    auto data = PackReader::instance().readFile(path);
    if (data.empty()) return false;

    SDL_IOStream* io = SDL_IOFromMem(data.data(), data.size());
    if (!io) {
        std::cerr << "[ERROR] SDL_IOFromMem failed for " << path << std::endl;
        return false;
    }

    SDL_Surface* surface = IMG_Load_IO(io, 0);
    SDL_CloseIO(io);

    if (!surface) {
        std::cerr << "[ERROR] IMG_Load_IO failed for " << path << ": " << SDL_GetError() << std::endl;
        return false;
    }

    auto tex = std::make_unique<Texture>();
    tex->m_renderer = renderer;
    tex->loadFromSurface(surface);
    SDL_DestroySurface(surface);

    if (!tex->getTexture()) {
        std::cerr << "[ERROR] SDL_CreateTextureFromSurface failed for " << path << ": " << SDL_GetError() << std::endl;
        return false;
    }

    textures.emplace(id, std::move(tex));
    std::cout << "[LOADED] Packed Texture: " << id << " from " << path << std::endl;
    return true;
}
