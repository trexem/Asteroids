#include "HealthSystem.h"

void HealthSystem::update(EntityManager* eManager, double dT) {
    HealthComponent hComp;
    for (uint32_t id : eManager->getEntitiesWithComponent(ComponentType::Health)) {
        hComp = eManager->getComponentData<HealthComponent>(id);
        if (hComp.regen == 0.0f || hComp.health == hComp.maxHealth) continue;
        hComp.health = std::min(hComp.regen * float(dT) + hComp.health, hComp.maxHealth);
        eManager->setComponentData(id, hComp);
    }
}
