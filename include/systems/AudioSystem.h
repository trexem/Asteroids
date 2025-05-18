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
    std::deque<Mix_Chunk*> toPlay;
    int masterVolume, musicVolume, sfxVolume;
    AudioSystem();
    ~AudioSystem();
    void update(EntityManager& eMgr, const double& dT) override;
    void setSfxVolume(int volume);
    void setMasterVolume(int volume);
    void setMusicVolume(int volume);
    void updateVolumes();
private:
    void handleExplodeMessage(std::shared_ptr<ExplodeMessage> msg);
    void handleAbilityMessage(std::shared_ptr<AbilityMessage> msg);
    void handleVolumeMessage(std::shared_ptr<VolumeMessage> msg);
};
