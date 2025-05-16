#include "LifeTimeSystem.h"
#include "GameStateManager.h"

LifeTimeSystem::LifeTimeSystem(EntityManager& eMgr) : eManager(eMgr) {

}

void LifeTimeSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        for (auto eID : eMgr.getEntitiesWithComponent(ComponentType::LifeTime)) {
            LifeTimeComponent lifeTime = eMgr.getComponentData<LifeTimeComponent>(eID);
            lifeTime.lifeTime -= dT;
            //std::cout << "LifeTime for: " << eID << " is " << lifeTime.lifeTime << std::endl;
            if (lifeTime.lifeTime <= 0) {
                TypeComponent* type = eMgr.getComponentDataPtr<TypeComponent>(eID);
                if (type->type & EntityType::Rocket || type->type & EntityType::Explosive) {
                    //std::cout << "Destroying rocket after lifeTime: " << eID << std::endl;
                    WeaponAbilities w = type->type == EntityType::Rocket ? WeaponAbilities::Rocket : WeaponAbilities::Explosives;
                    MessageManager::instance().sendMessage(std::make_shared<ExplodeMessage>(eID, w));
                } else {
                    //std::cout << "Destroying explosion: " << eID << std::endl;
                    eMgr.destroyEntityLater(eID);
                }
                return;
            }
            eMgr.setComponentData(eID, lifeTime);
        }
    }
}
