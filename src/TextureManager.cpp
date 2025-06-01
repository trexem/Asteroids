#include "TextureManager.h"
#include "PackReader.h"
#include "utils.hpp"
#include "Log.h"

#include <filesystem>
#include <algorithm>

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
    keys.push_back(id);
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
        DEBUG_LOG("Texture ID already exists (from pack): %s", id.c_str());
        return false; 
    }

    auto data = PackReader::instance().readFile(path);
    if (data.empty()) return false;

    SDL_IOStream* io = SDL_IOFromMem(data.data(), data.size());
    if (!io) {
        DEBUG_LOG("[ERROR] SDL_IOFromMem failed for %s", path.c_str());
        return false;
    }

    SDL_Surface* surface = IMG_Load_IO(io, 0);
    SDL_CloseIO(io);

    if (!surface) {
        DEBUG_LOG("[ERROR] IMG_Load_IO failed for %s: %s", path.c_str(), SDL_GetError());
        return false;
    }

    auto tex = std::make_unique<Texture>();
    tex->m_renderer = renderer;
    tex->loadFromSurface(surface);
    SDL_DestroySurface(surface);

    if (!tex->getTexture()) {
        DEBUG_LOG("[ERROR] SDL_CreateTextureFromSurface failed for %s: %s", path.c_str(), SDL_GetError());
        return false;
    }

    textures.emplace(id, std::move(tex));
    keys.push_back(id);
    DEBUG_LOG("[LOADED] Packed Texture: %s from %s", id.c_str(), path.c_str());
    return true;
}

std::vector<Texture*> TextureManager::getAnimationFrames(const std::string& fullPrefix) {
    std::vector<std::string> matching;
    for (const auto& k : keys) {
        if (k.starts_with(fullPrefix)) {
            matching.push_back(k);
        }
    }

    std::sort(matching.begin(), matching.end());

    std::vector<Texture*> frames;
    for (const auto& k : matching) {
        if (auto tex = get(k)) {
            frames.push_back(tex);
        }
    }

    if (frames.empty()) {
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "No animation frames found for %s", fullPrefix.c_str());
    }

    return frames;
}

std::vector<Texture*> TextureManager::getAnimationFrames(const std::string& key, const std::string& prefix) {
    std::vector<Texture*> frames;
    if (auto tex = get(key)) {
        frames.push_back(tex);
    }
    std::string fullPrefix = key + prefix;
    std::vector<Texture*> f = getAnimationFrames(fullPrefix);
    frames.insert(frames.end(), f.begin(), f.end());
    if (auto tex = get(key)) {
        frames.push_back(tex);
    }
    return frames;
}
