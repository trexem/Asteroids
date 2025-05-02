#include "LifeTimeSystem.h"

LifeTimeSystem::LifeTimeSystem(EntityManager* eMgr) : eManager(eMgr) {

}

void LifeTimeSystem::update(double dT) {
    for (auto eID : eManager->getEntitiesWithComponent(ComponentType::LifeTime)) {
        LifeTimeComponent lifeTime = eManager->getComponentData<LifeTimeComponent>(eID);
        lifeTime.lifeTime -= dT;
        //std::cout << "LifeTime for: " << eID << " is " << lifeTime.lifeTime << std::endl;
        if (lifeTime.lifeTime <= 0) {
            TypeComponent* type = eManager->getComponentDataPtr<TypeComponent>(eID);
            if (type->type & EntityType::Rocket || type->type & EntityType::Explosive) {
                //std::cout << "Destroying rocket after lifeTime: " << eID << std::endl;
                WeaponAbilities w = type->type == EntityType::Rocket ? WeaponAbilities::Rocket : WeaponAbilities::Explosives;
                MessageManager::instance().sendMessage(std::make_shared<ExplodeMessage>(eID, w));
            } else {
                //std::cout << "Destroying explosion: " << eID << std::endl;
                eManager->destroyEntityLater(eID);
            }
            return;
        }
        eManager->setComponentData(eID, lifeTime);
    }
}
