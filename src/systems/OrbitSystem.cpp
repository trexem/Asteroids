#include "OrbitSystem.h"

void OrbitSystem::update(double dT) {
    for (uint32_t id : eManager->getEntitiesWithComponent(ComponentType::Orbit)) {
        OrbitComponent orbitComp = eManager->getComponentData<OrbitComponent>(id);
        TransformComponent orbitTrans = eManager->getComponentData<TransformComponent>(id);
        TransformComponent* parentTrans = eManager->getComponentDataPtr<TransformComponent>(orbitComp.parentId);

        orbitComp.angle += dT * orbitComp.rotationSpeed;
        if (orbitComp.angle > TAU) {
            orbitComp.angle -= TAU;
        }
        float offsetX = orbitComp.radius * sin(orbitComp.angle);
        float offsetY = orbitComp.radius * cos(orbitComp.angle);
        orbitTrans.position.x = parentTrans->position.x + offsetX;
        orbitTrans.position.y = parentTrans->position.y + offsetY;
        eManager->setComponentData(id, orbitComp);
        eManager->setComponentData(id, orbitTrans);
    }
}
