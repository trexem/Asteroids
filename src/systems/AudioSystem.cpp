#include "AudioSystem.h"
#include "MessageManager.h"
#include "ExplodeMessage.h"
#include "AbilityMessage.h"
#include "VolumeMessage.h"
#include "SettingsManager.h"
#include "AudioManager.h"

AudioSystem::AudioSystem() {
    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
    spec.freq = MIX_DEFAULT_FREQUENCY;
    spec.format = MIX_DEFAULT_FORMAT;
    spec.channels = MIX_DEFAULT_CHANNELS;
    if (!Mix_OpenAudio(0, &spec)) {
        std::cerr << "SDL_Mixer could not open audio! SDL_ERROR: " << SDL_GetError() << '\n';
    } else {

    }

    AudioManager::instance().loadAllFromPack("sounds/");
    MessageManager::instance().subscribe<ExplodeMessage>(
        [this](std::shared_ptr<ExplodeMessage> msg) { handleExplodeMessage(msg); }
    );
    MessageManager::instance().subscribe<AbilityMessage>(
        [this](std::shared_ptr<AbilityMessage> msg) { handleAbilityMessage(msg); }
    );
    MessageManager::instance().subscribe<VolumeMessage>(
        [this](std::shared_ptr<VolumeMessage> msg) { handleVolumeMessage(msg); }
    );
    GameSettings s = SettingsManager::instance().get();
    masterVolume = s.masterVolume;
    musicVolume = s.musicVolume;
    sfxVolume = s.sfxVolume;
    updateVolumes();
}

AudioSystem::~AudioSystem() {
    AudioManager::instance().clear();
    Mix_CloseAudio();
    Mix_Quit();
}

void AudioSystem::handleExplodeMessage(std::shared_ptr<ExplodeMessage> msg) {
    toPlay.push_back(AudioManager::instance().getSound("explosion"));
}

void AudioSystem::handleAbilityMessage(std::shared_ptr<AbilityMessage> msg) {
    std::string id = weaponId[static_cast<size_t>(msg->ability)];
    toPlay.push_back(AudioManager::instance().getSound(id));
}

void AudioSystem::update(EntityManager& eMgr, const double& dT) {
    while (!toPlay.empty()) {
        Mix_PlayChannel(-1, toPlay.front(), 0);
        toPlay.pop_front();
    }
}

void AudioSystem::setSfxVolume(int volume) {
    AudioManager::instance().setSoundsVolume(volume);
}

void AudioSystem::setMasterVolume(int volume) {
    Mix_Volume(-1, volume);
}

void AudioSystem::setMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}

void AudioSystem::handleVolumeMessage(std::shared_ptr<VolumeMessage> msg) {
    switch (msg->soruce) {
    case VolumeSource::MasterVolume:
        masterVolume = msg->volume;
        break;
    case VolumeSource::MusicVolume:
        musicVolume = msg->volume;
        break;
    case VolumeSource::SFXVolume:
        sfxVolume = msg->volume;
        break;
    }
    updateVolumes();
}

void AudioSystem::updateVolumes() {
    int music = static_cast<int>(masterVolume / 10.0 * musicVolume / 10.0 * MIX_MAX_VOLUME);
    int sfx = static_cast<int>(masterVolume / 10.0 * sfxVolume / 10.0 * MIX_MAX_VOLUME);

    Mix_VolumeMusic(music);
    Mix_Volume(-1, sfx);
}
