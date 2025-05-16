#include "OrbitSystem.h"
#include "GameStateManager.h"

void OrbitSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        for (uint32_t id : eMgr.getEntitiesWithComponent(ComponentType::Orbit)) {
            OrbitComponent orbitComp = eMgr.getComponentData<OrbitComponent>(id);
            TransformComponent orbitTrans = eMgr.getComponentData<TransformComponent>(id);
            TransformComponent* parentTrans = eMgr.getComponentDataPtr<TransformComponent>(orbitComp.parentId);

            orbitComp.angle += dT * orbitComp.rotationSpeed;
            if (orbitComp.angle > TAU) {
                orbitComp.angle -= TAU;
            }
            float offsetX = orbitComp.radius * sin(orbitComp.angle);
            float offsetY = orbitComp.radius * cos(orbitComp.angle);
            orbitTrans.position.x = parentTrans->position.x + offsetX;
            orbitTrans.position.y = parentTrans->position.y + offsetY;
            eMgr.setComponentData(id, orbitComp);
            eMgr.setComponentData(id, orbitTrans);
        }
    }
}
