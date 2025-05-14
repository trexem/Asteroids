#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <deque>
#include <unordered_map>

#include "System.h"

class ExplodeMessage;
class AbilityMessage;
class VolumeMessage;

class AudioSystem : public System {
public:
    SDL_AudioSpec spec;
    Mix_Chunk *explosion;
    std::unordered_map<WeaponAbilities, Mix_Chunk*> sounds;
    Mix_Music *music;
    std::deque<Mix_Chunk*> toPlay;
    int masterVolume, musicVolume, sfxVolume;
    AudioSystem();
    ~AudioSystem();
    void update();
    void setSfxVolume(int volume);
    void setMasterVolume(int volume);
    void setMusicVolume(int volume);
    void updateVolumes();
private:
    void handleExplodeMessage(std::shared_ptr<ExplodeMessage> msg);
    void handleAbilityMessage(std::shared_ptr<AbilityMessage> msg);
    void handleVolumeMessage(std::shared_ptr<VolumeMessage> msg);
};
