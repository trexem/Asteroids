#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <deque>

#include "System.h"

class ExplodeMessage;
class AbilityMessage;

class AudioSystem : public System {
public:
    SDL_AudioSpec spec;
    Mix_Chunk *laserGunChunk, *rocketChunk, *laserChunk;
    Mix_Chunk *explosiveChunk, *explosionChunk, *gravitySawChunk;
    Mix_Music *music;
    std::deque<Mix_Chunk*> toPlay;
    AudioSystem();
    ~AudioSystem();
    void update();
private:
    void handleExplodeMessage(std::shared_ptr<ExplodeMessage> msg);
    void handleAbilityMessage(std::shared_ptr<AbilityMessage> msg);
};
