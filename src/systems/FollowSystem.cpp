#include "FollowSystem.h"

void FollowSystem::update(EntityManager* eManager) {
    for (uint32_t id : eManager->getEntitiesWithComponent(ComponentType::Follow)) {
        TransformComponent trans = eManager->getComponentData<TransformComponent>(id);
        FollowComponent* follow = eManager->getComponentDataPtr<FollowComponent>(id);
        TransformComponent* parentTrans = eManager->getComponentDataPtr<TransformComponent>(follow->parentId);
        RenderComponent* rComp = eManager->getComponentDataPtr<RenderComponent>(id);
        double angle = follow->offsetAngle + parentTrans->rotDegrees;
        double radians = angle * DEG2RAD;
        FPair rotatedOffset = {
            float(follow->offsetPosition.x + follow->offsetPosition.y * sin(radians)),
            float(follow->offsetPosition.y - follow->offsetPosition.y * cos(radians)) - rComp->exactSize.y
        };
        trans.position = parentTrans->position + rotatedOffset;
        trans.rotDegrees = angle;
        eManager->setComponentData(id, trans);
    }
}
