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
    sounds[WeaponAbilities::LaserGun] = Mix_LoadWAV("data/sounds/laserGun.wav");
    if (!sounds[WeaponAbilities::LaserGun]) std::cerr << "Failed to load laserGun.wav: " << SDL_GetError() << '\n';
    
    sounds[WeaponAbilities::Rocket] = Mix_LoadWAV("data/sounds/rocket.wav");
    if (!sounds[WeaponAbilities::Rocket]) std::cerr << "Failed to load rocket.wav: " << SDL_GetError() << '\n';
    
    sounds[WeaponAbilities::Explosives] = Mix_LoadWAV("data/sounds/explosive.wav");
    if (!sounds[WeaponAbilities::Explosives]) std::cerr << "Failed to load explosive.wav: " << SDL_GetError() << '\n';
    
    sounds[WeaponAbilities::Laser] = Mix_LoadWAV("data/sounds/laser.wav");
    if (!sounds[WeaponAbilities::Laser]) std::cerr << "Failed to load laser.wav: " << SDL_GetError() << '\n';
    
    sounds[WeaponAbilities::GravitySaws] = Mix_LoadWAV("data/sounds/gravitySaw.wav");
    if (!sounds[WeaponAbilities::GravitySaws]) std::cerr << "Failed to load gravitySaw.wav: " << SDL_GetError() << '\n';
    
    explosion = Mix_LoadWAV("data/sounds/explosion.wav");
    if (!explosion) std::cerr << "Failed to load explosion.wav: " << SDL_GetError() << '\n';

    MessageManager::instance().subscribe<ExplodeMessage>(
        [this](std::shared_ptr<ExplodeMessage> msg) { handleExplodeMessage(msg); }
    );
    MessageManager::instance().subscribe<AbilityMessage>(
        [this](std::shared_ptr<AbilityMessage> msg) { handleAbilityMessage(msg); }
    );
}

AudioSystem::~AudioSystem() {
    for (auto c : sounds) {
        Mix_FreeChunk(c.second);
    }
    Mix_FreeChunk(explosion);
    Mix_CloseAudio();
    Mix_Quit();
}

void AudioSystem::handleExplodeMessage(std::shared_ptr<ExplodeMessage> msg) {
    toPlay.push_back(explosion);
}

void AudioSystem::handleAbilityMessage(std::shared_ptr<AbilityMessage> msg) {
    if (auto it = sounds.find(msg->ability); it != sounds.end()) {
        toPlay.push_back(it->second);
    }
}

void AudioSystem::update() {
    while (!toPlay.empty()) {
        Mix_PlayChannel(-1, toPlay.front(), 0);
        toPlay.pop_front();
    }
}

void AudioSystem::setSfxVolume(int volume) {
    for (auto c : sounds) {
        Mix_VolumeChunk(c.second, volume);
    }
    Mix_VolumeChunk(explosion, volume);
}

void AudioSystem::setMasterVolume(int volume) {
    Mix_Volume(-1, volume);
}

void AudioSystem::setMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}
