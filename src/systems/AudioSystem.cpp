#include "AudioSystem.h"
#include "MessageManager.h"
#include "ExplodeMessage.h"
#include "AbilityMessage.h"

AudioSystem::AudioSystem() {
    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
    spec.freq = MIX_DEFAULT_FREQUENCY;
    spec.format = MIX_DEFAULT_FORMAT;
    spec.channels = MIX_DEFAULT_CHANNELS;
    if (!Mix_OpenAudio(0, &spec)) {
        std::cerr << "SDL_Mixer could not open audio! SDL_ERROR: " << SDL_GetError() << '\n';
    } else {

    }

    //Load wav and mp3
    laserGunChunk = Mix_LoadWAV("data/sounds/laserGun.wav");
    if (!laserGunChunk) std::cerr << "Failed to load laserGun.wav: " << SDL_GetError() << '\n';
    
    rocketChunk = Mix_LoadWAV("data/sounds/rocket.wav");
    if (!rocketChunk) std::cerr << "Failed to load rocket.wav: " << SDL_GetError() << '\n';
    
    explosiveChunk = Mix_LoadWAV("data/sounds/explosive.wav");
    if (!explosiveChunk) std::cerr << "Failed to load explosive.wav: " << SDL_GetError() << '\n';
    
    laserChunk = Mix_LoadWAV("data/sounds/laser.wav");
    if (!laserChunk) std::cerr << "Failed to load laser.wav: " << SDL_GetError() << '\n';
    
    gravitySawChunk = Mix_LoadWAV("data/sounds/gravitySaw.wav");
    if (!gravitySawChunk) std::cerr << "Failed to load gravitySaw.wav: " << SDL_GetError() << '\n';
    
    explosionChunk = Mix_LoadWAV("data/sounds/explosion.wav");
    if (!explosionChunk) std::cerr << "Failed to load explosion.wav: " << SDL_GetError() << '\n';

    MessageManager::instance().subscribe<ExplodeMessage>(
        [this](std::shared_ptr<ExplodeMessage> msg) { handleExplodeMessage(msg); }
    );
    MessageManager::instance().subscribe<AbilityMessage>(
        [this](std::shared_ptr<AbilityMessage> msg) { handleAbilityMessage(msg); }
    );
}

AudioSystem::~AudioSystem() {
    Mix_FreeChunk(laserGunChunk);
    Mix_FreeChunk(rocketChunk);
    Mix_FreeChunk(laserChunk);
    Mix_FreeChunk(explosionChunk);
    Mix_FreeChunk(explosiveChunk);
    Mix_FreeChunk(gravitySawChunk);
    Mix_CloseAudio();
    Mix_Quit();
}

void AudioSystem::handleExplodeMessage(std::shared_ptr<ExplodeMessage> msg) {
    toPlay.push_back(explosionChunk);
}

void AudioSystem::handleAbilityMessage(std::shared_ptr<AbilityMessage> msg) {
    switch (msg->ability) {
    case WeaponAbilities::LaserGun:
        toPlay.push_back(laserGunChunk);
        break;
    case WeaponAbilities::Rocket:
        toPlay.push_back(rocketChunk);    
        break;
    case WeaponAbilities::Laser:
        toPlay.push_back(laserChunk);    
        break;
    case WeaponAbilities::GravitySaws:
        toPlay.push_back(gravitySawChunk);    
        break;
    case WeaponAbilities::Explosives:
        toPlay.push_back(explosiveChunk);    
        break;
    default:
        break;
    }
}

void AudioSystem::update() {
    while (!toPlay.empty()) {
        Mix_PlayChannel(-1, toPlay.front(), 0);
        toPlay.pop_front();
    }
}
