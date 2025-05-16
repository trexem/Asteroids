#include "FollowSystem.h"
#include "GameStateManager.h"

void FollowSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        for (uint32_t id : eMgr.getEntitiesWithComponent(ComponentType::Follow)) {
            FollowComponent follow = eMgr.getComponentData<FollowComponent>(id);
            if (eMgr.entityExists(follow.parentId) && 
                !eMgr.isMarkedForDestruction(follow.parentId)
            ) {
                TransformComponent trans = eMgr.getComponentData<TransformComponent>(id);
                TransformComponent* parentTrans = eMgr.getComponentDataPtr<TransformComponent>(follow.parentId);
                RenderComponent* rComp = eMgr.getComponentDataPtr<RenderComponent>(id);
                double angle = follow.offsetAngle + parentTrans->rotDegrees;
                double radians = angle * DEG2RAD;
                FPair rotatedOffset = {
                    float(follow.offsetPosition.x + follow.offsetPosition.y * sin(radians)),
                    float(follow.offsetPosition.y - follow.offsetPosition.y * cos(radians)) - rComp->exactSize.y
                };
                trans.position = parentTrans->position + rotatedOffset;
                trans.rotDegrees = angle;
                eMgr.setComponentData(id, trans);
            } else {
                eMgr.removeComponent(id, ComponentType::Follow);
            }
        }
    }
}
