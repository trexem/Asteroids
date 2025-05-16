#include "HealthSystem.h"
#include "GameStateManager.h"

void HealthSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        HealthComponent hComp;
        for (uint32_t id : eMgr.getEntitiesWithComponent(ComponentType::Health)) {
            hComp = eMgr.getComponentData<HealthComponent>(id);
            if (hComp.regen == 0.0f || hComp.health == hComp.maxHealth) continue;
            hComp.health = std::min(hComp.regen * float(dT) + hComp.health, hComp.maxHealth);
            eMgr.setComponentData(id, hComp);
        }
    }
}
