#pragma once

#include "Screen.h"

class Texture;

class SettingsScreen : public Screen {

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

public:
    SettingsScreen(EntityManager* eManager) : Screen(eManager) {}
    void create(EntityManager* eManager, SDL_Renderer* renderer);
    void destroy(EntityManager* eManager);
    void update(EntityManager* eManager, SDL_Renderer* renderer);
};
