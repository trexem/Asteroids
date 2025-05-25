#pragma once

#include "Screen.h"

class Texture;
class Button;
class Label;

class MainMenuScreen : public Screen {
    std::unique_ptr<Button> playButton;
    std::unique_ptr<Button> settingsButton;
    std::unique_ptr<Button> quitButton;
    std::unique_ptr<Button> upgradesButton;
    std::unique_ptr<Label> goldLabel;
    Texture goldTexture;
    uint32_t playID, upgradesID, settingsID, quitID, goldID;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);
    void onPlayClick();

public:
    MainMenuScreen(EntityManager& eManager) : Screen(eManager) {}
    ~MainMenuScreen();
    void create(EntityManager& eManager, SDL_Renderer* renderer);
    void destroy(EntityManager& eManager);
    void update(EntityManager& eManager, SDL_Renderer* renderer);
};
