#pragma once

#include "Screen.h"

class Texture;

class MainMenuScreen : public Screen {
    Texture playTexture;
    Texture settingsTexture;
    Texture quitTexture;
    Texture upgradesTexture;
    Texture goldTexture;
    uint32_t playID, upgradesID, settingsID, quitID, goldID;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);
    void onPlayClick();

public:
    MainMenuScreen(EntityManager* eManager) : Screen(eManager) {}
    ~MainMenuScreen();
    void create(EntityManager* eManager, SDL_Renderer* renderer);
    void destroy(EntityManager* eManager);
    void update(EntityManager* eManager, SDL_Renderer* renderer);
};
