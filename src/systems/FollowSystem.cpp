#include "FollowSystem.h"

void FollowSystem::update(EntityManager* eManager) {
    for (uint32_t id : eManager->getEntitiesWithComponent(ComponentType::Follow)) {
        TransformComponent trans = eManager->getComponentData<TransformComponent>(id);
        FollowComponent* follow = eManager->getComponentDataPtr<FollowComponent>(id);
        TransformComponent* parentTrans = eManager->getComponentDataPtr<TransformComponent>(follow->parentId);
        double angle = follow->offsetAngle + parentTrans->rotDegrees;
        double radians = angle * DEG2RAD;
        FPair rotatedOffset = {
            float(follow->offsetPosition.x * cos(radians) - follow->offsetPosition.y * sin(radians)),
            float(follow->offsetPosition.x * sin(radians) + follow->offsetPosition.y * cos(radians))
        };
        std::cout << "Game Angle: " << angle << "°"
          << " | Offset: " << rotatedOffset 
          << " | Final Pos: " << trans.position << "\n";
        trans.position = parentTrans->position + rotatedOffset;
        trans.rotDegrees = angle;
        eManager->setComponentData(id, trans);
    }
}
