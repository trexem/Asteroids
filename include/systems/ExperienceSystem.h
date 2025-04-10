#pragma once

#include "ExperienceSpawnMessage.h"
#include "MessageManager.h"
#include "System.h"
#include "texture.hpp"
#include "utils.hpp"

class ExperienceSystem : public System {
public:
    ExperienceSystem(EntityManager* eMgr, SDL_Renderer* renderer);
    void update();

private:
    void handleExperienceSpawnMessage(std::shared_ptr<ExperienceSpawnMessage> msg);
    EntityManager* eManager;
    Texture experienceTexture;
};
