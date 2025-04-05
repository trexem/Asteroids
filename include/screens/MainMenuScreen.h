#pragma once

#include "Colors.h"
#include "Fonts.h"
#include "Screen.h"
#include "texture.hpp"
#include "utils.hpp"

class MainMenuScreen : public Screen {
    Texture playTexture;
    Texture settingsTexture;
    Texture quitTexture;
    uint32_t playID, settingsID, quitID;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);
    void onPlayClick();

public:
    MainMenuScreen(EntityManager* eManager) : Screen(eManager) {
        std::cout << "Creating MainMenuScreen\n";
    }
    ~MainMenuScreen();
    void create(EntityManager* eManager, SDL_Renderer* renderer);
    void destroy(EntityManager* eManager);
};
