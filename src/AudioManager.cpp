#include "AudioManager.h"
#include "PackReader.h"
#include "utils.hpp"

#include <iostream>

AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::loadSound(const std::string& id, const std::string& path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        std::cerr << "Failed to load sound: " << path << " — " << SDL_GetError() << std::endl;
        return false;
    }
    soundMap[id] = chunk;
    return true;
}

bool AudioManager::loadMusic(const std::string& id, const std::string& path) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << path << " — " << SDL_GetError() << std::endl;
        return false;
    }
    musicMap[id] = music;
    return true;
}

bool AudioManager::loadSoundFromPack(const std::string& id, const std::string& virtualPath) {
    auto data = PackReader::instance().readFile(normalizePath(virtualPath));
    if (data.empty()) return false;

    SDL_IOStream* io = SDL_IOFromMem(data.data(), data.size());
    Mix_Chunk* chunk = Mix_LoadWAV_IO(io, 0);
    SDL_CloseIO(io);

    if (!chunk) {
        std::cerr << "Failed to load sound from pack: " << virtualPath << std::endl;
        return false;
    }
    soundMap[id] = chunk;
    return true;
}

bool AudioManager::loadMusicFromPack(const std::string& id, const std::string& virtualPath) {
    auto data = PackReader::instance().readFile(normalizePath(virtualPath));
    if (data.empty()) return false;

    SDL_IOStream* io = SDL_IOFromMem(data.data(), data.size());
    Mix_Music* music = Mix_LoadMUS_IO(io, 0);
    SDL_CloseIO(io);

    if (!music) {
        std::cerr << "Failed to load music from pack: " << virtualPath << std::endl;
        return false;
    }
    musicMap[id] = music;
    return true;
}

Mix_Chunk* AudioManager::getSound(const std::string& id) {
    auto it = soundMap.find(id);
    return it != soundMap.end() ? it->second : nullptr;
}

Mix_Music* AudioManager::getMusic(const std::string& id) {
    auto it = musicMap.find(id);
    return it != musicMap.end() ? it->second : nullptr;
}

void AudioManager::clear() {
    for (auto& [_, chunk] : soundMap) {
        Mix_FreeChunk(chunk);
    }
    soundMap.clear();

    for (auto& [_, music] : musicMap) {
        Mix_FreeMusic(music);
    }
    musicMap.clear();
}

void AudioManager::loadAllFromPack(const std::string& prefix) {
    auto files = PackReader::instance().listFiles(prefix);
    for (const auto& path : files) {
        std::string id = normalizePath(path.substr(prefix.length()));
        size_t dot = id.find_last_of('.');
        std::string ext = (dot != std::string::npos) ? id.substr(dot + 1) : "";
        if (dot != std::string::npos) id = id.substr(0, dot);

        if (ext == "ogg" || ext == "mp3" || ext == "wav") {
            if (ext == "wav") {
                loadSoundFromPack(id, path);
            } else {
                loadMusicFromPack(id, path);
            }
        }
    }
}

void AudioManager::setSoundsVolume(int volume) {
    for (auto s : soundMap) {
        Mix_VolumeChunk(s.second, volume);
    }
}
