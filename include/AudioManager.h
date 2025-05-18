#pragma once

#include <string>
#include <unordered_map>

#include <SDL3_mixer/SDL_mixer.h>

class AudioManager {
public:
    static AudioManager& instance();

    bool loadSound(const std::string& id, const std::string& path);
    bool loadMusic(const std::string& id, const std::string& path);

    bool loadSoundFromPack(const std::string& id, const std::string& virtualPath);
    bool loadMusicFromPack(const std::string& id, const std::string& virtualPath);

    Mix_Chunk* getSound(const std::string& id);
    Mix_Music* getMusic(const std::string& id);

    void setSoundsVolume(int volume);

    void loadAllFromPack(const std::string& prefix);
    void clear();

private:
    AudioManager() = default;

    std::unordered_map<std::string, Mix_Chunk*> soundMap;
    std::unordered_map<std::string, Mix_Music*> musicMap;
};
