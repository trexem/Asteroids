#pragma once

#include <vector>
#include <memory>

#include "Screen.h"
#include "LevelUpContainer.h"
#include "LevelUpMessage.h"
#include "MessageManager.h"

class LevelUpScreen : public Screen {
    Texture container1;
    Texture container2;
    Texture container3;
    uint32_t id1, id2, id3;
    std::vector<std::unique_ptr<LevelUpContainer>> lvlUpContainers;
    std::vector<ShipAbilities> options;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

    void drawConatiner(SDL_Renderer* renderer, SDL_Texture* texture);
    void onContainerClick(int i);

public:
    LevelUpScreen(EntityManager* eManager) : Screen(eManager) {}
    void create(EntityManager* eManager, SDL_Renderer* renderer);
    void destroy(EntityManager* eManager);
    void update(EntityManager* eManager, SDL_Renderer* renderer);
};
