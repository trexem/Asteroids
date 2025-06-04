#pragma once


#include "Screen.h"
#include "GameSettings.h"
#include "screens/Components/Button.h"
#include "screens/Components/Label.h"
#include "screens/Components/Box.h"

class Texture;
using GameSettings = GameSave::GameSettings;

class SettingsScreen : public Screen {
    std::shared_ptr<Button> leftResolutionButton, rightResolutionButton; 
    std::shared_ptr<Button> leftMasterVolume, rightMasterVolume;
    std::shared_ptr<Button> leftMusicVolume, rightMusicVolume;
    std::shared_ptr<Button> leftSfxVolume, rightSfxVolume;
    std::shared_ptr<Button> backButton;
    std::shared_ptr<Box> fullScreenButton, vsyncButton;
    std::shared_ptr<Label> resolutionLabel, fullscreenLabel;
    std::shared_ptr<Label> vsyncLabel, masterLabel;
    std::shared_ptr<Label> musicLabel, sfxLabel;
    Texture *backButtonTexture, *leftButtonTexture, *rightButtonTexture;
    Texture *checkBoxFalseTexture, *checkBoxTrueTexture;
    std::string resolutionText, masterText, musicText, sfxText;
    GameSettings settings;
    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

public:
    SettingsScreen(EntityManager& eManager) : Screen(eManager) {}
    void create(EntityManager& eManager, SDL_Renderer* renderer);
    void destroy(EntityManager& eManager);
    void update(EntityManager& eManager, SDL_Renderer* renderer);
};
