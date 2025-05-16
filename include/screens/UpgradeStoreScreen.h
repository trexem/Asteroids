#pragma once

#include <vector>
#include <memory>

#include "screens/Components/UpgradeButton.h"
#include "Screen.h"

class Texture;

class UpgradeStoreScreen : public Screen {
    Texture containerTexture;
    Texture pressedButtonTexture;
    Texture idleButtonTexture;
    Texture backButtonTexture;
    std::vector<std::shared_ptr<UpgradeButton>> upgradeButtons;
    std::shared_ptr<Button> backButton;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

public:
    UpgradeStoreScreen(EntityManager& eManager, SDL_Renderer* renderer);
    void create(EntityManager& eManager, SDL_Renderer* renderer);
    void destroy(EntityManager& eManager);
    void update(EntityManager& eManager, SDL_Renderer* renderer);
};
