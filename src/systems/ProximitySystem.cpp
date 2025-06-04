#include "ProximitySystem.h"
#include "SpatialGridBuilder.h"
#include "Log.h"

#include <chrono>

void ProximitySystem::update(EntityManager& eMgr, const double& dT) {
    std::vector<bool> visited(eMgr.maxEntities, false);
    auto proxEntities = eMgr.getEntitiesWithComponent(ComponentType::ProximityTracker);
    DEBUG_LOG("Prox entities: %d", proxEntities.size());
    
    for (auto seeker : proxEntities) {
        auto* proxComp = eMgr.getComponentDataPtr<ProximityTrackerComponent>(seeker);
        auto* seekerCol = eMgr.getComponentDataPtr<CollisionComponent>(seeker);
        auto seekerPos = eMgr.getComponentData<TransformComponent>(seeker).position;
        if (!proxComp) {
            DEBUG_LOG("No Prox Comp");
            continue;
        };

        float seekerRadius = seekerCol->getBoundingRadius();

        proxComp->closest = std::nullopt;
        proxComp->distance = std::numeric_limits<float>::max();

        auto start = std::chrono::high_resolution_clock::now();
        const auto& candidates = SpatialGrid::instance().getNearbyEntities(seekerPos, maxRange, visited);
        auto end = std::chrono::high_resolution_clock::now();
        // DEBUG_LOG("getNearbyEntities update time: %lld us", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        
        start = std::chrono::high_resolution_clock::now();
        for (auto target : candidates) {
            if (target == seeker) continue;
            if (proxComp->closest.has_value()) {
                if (target == *proxComp->closest) continue;
            }
            if (!eMgr.hasComponent<TypeComponent>(target)) continue;

            auto targetType = eMgr.getComponentData<TypeComponent>(target).type;
            if (targetType != EntityType::Asteroid) continue;

            auto targetCol = eMgr.getComponentData<CollisionComponent>(target);
            float targetRadius = targetCol.getBoundingRadius();
            const auto& targetPos = eMgr.getComponentData<TransformComponent>(target).position;

            float distSq = getSquaredDistanceBetweenCircles(seekerPos, seekerRadius, targetPos, targetRadius);

            if (distSq < maxRangeSq && distSq < proxComp->distance) {
                proxComp->closest = target;
                proxComp->distance = distSq;
                // DEBUG_LOG("Closest: %d with distance %f", target, distSq);
            }
        }
        end = std::chrono::high_resolution_clock::now();
        // DEBUG_LOG("target for loop update time: %lld us", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }
}
