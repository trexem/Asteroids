#pragma once

#include <vector>
#include <memory>

#include "screens/Components/UpgradeButton.h"
#include "screens/Components/Label.h"
#include "Screen.h"

class Texture;

class UpgradeStoreScreen : public Screen {
    std::vector<std::shared_ptr<UpgradeButton>> upgradeButtons;
    std::shared_ptr<Button> backButton;
    std::unique_ptr<Label> goldLabel;
    uint32_t gold {0};

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

public:
    UpgradeStoreScreen(EntityManager& eManager, SDL_Renderer* renderer);
    void create(EntityManager& eManager, SDL_Renderer* renderer);
    void destroy(EntityManager& eManager);
    void update(EntityManager& eManager, SDL_Renderer* renderer);
};
