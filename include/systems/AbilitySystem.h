#pragma once

#include "abilities.h"
#include "AbilityMessage.h"
#include "DestroyRocketMessage.h"
#include "MessageManager.h"
#include "System.h"
#include "utils.hpp"

class AbilitySystem : public System {
public:
    AbilitySystem(EntityManager* eManager, SDL_Renderer* renderer);
    ~AbilitySystem();
    void update();
    
    private:
    void handleAbilityMessage(std::shared_ptr<AbilityMessage> msg);
    void handleDestroyRocketMessage(std::shared_ptr<DestroyRocketMessage> msg);
    void createExplosion(const FPair& pos);
    void spawnLaserGun(uint32_t eID);
    void spawnRocket(uint32_t eID);
    EntityManager* eManager;
    std::vector<FPair> rocketsDestroyed;
    Texture explosionTexture;
};
