#pragma once
#ifndef __ENTITY_MANAGER_H_
#define __ENTITY_MANAGER_H_

#include <vector>
#include <bitset>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <cassert>
#include "Components.h"

class EntityManager {
public:
    EntityManager(size_t t_maxEntities);
    ~EntityManager();
    uint32_t createEntity();
    void destroyEntity(uint32_t entityID);
    const bool entityExists(uint32_t entityID) const {
        return entityID < maxEntities && entityComponentMasks[entityID].any();
    }

    void addComponent(uint32_t entityID, ComponentType type);
    void addComponents(uint32_t entityID, const std::vector<ComponentType>& types);
    void removeComponent(uint32_t entityID, ComponentType type);
    void removeComponents(uint32_t entityID, const std::vector<ComponentType>& types);

    template <typename T>
    void setComponentData(uint32_t entityID, const T& componentData) {
        static_assert(std::is_standard_layout<T>::value, "Component type must be standard layout.");
        assert(entityID < maxEntities);
        
        ComponentType type = getComponentType<T>();
        if (entityComponentMasks[entityID][static_cast<size_t>(type)]) {
            if (entityID < componentPools[static_cast<size_t>(type)].size()) {
                // Create and store a copy of the component data
                componentPools[static_cast<size_t>(type)][entityID] = std::make_unique<T>(componentData);
            }
        }
    }

    template <typename T>
    const T& getComponentData(uint32_t entityID) const {
        const ComponentType type = getComponentType<T>();
        if (entityComponentMasks[entityID][static_cast<size_t>(type)]) {
            if (entityID < componentPools[static_cast<size_t>(type)].size()) {
                if (componentPools[static_cast<size_t>(type)][entityID]) {
                    auto& componentData = *static_cast<T*>(componentPools[static_cast<size_t>(type)][entityID].get());
                    return componentData;
                }
            }
        }
        // Handle the case where the component data does not exist.
        throw std::runtime_error("Component data does not exist for the specified entity.");
    }

    template <typename T>
    bool hasComponent(uint32_t entityID) const {
        ComponentType type = getComponentType<T>();
        return entityComponentMasks[entityID][static_cast<size_t>(type)];
    }

    std::vector<uint32_t> getEntitiesWithComponent(ComponentType type);
private:
    uint32_t findAvailableEntityID();

    template <typename T>
    ComponentType getComponentType() const {
        if constexpr (std::is_same<T, TransformComponent>::value) {
            return ComponentType::Transform;
        } else if constexpr (std::is_same<T, PhysicsComponent>::value) {
            return ComponentType::Physics;
        } else if constexpr (std::is_same<T, CollisionComponent>::value) {
            return ComponentType::Collision;
        } else if constexpr (std::is_same<T, RenderComponent>::value) {
            return ComponentType::Render;
        } else if constexpr (std::is_same<T, HealthComponent>::value) {
            return ComponentType::Health;
        } else if constexpr (std::is_same<T, DamageComponent>::value) {
            return ComponentType::Damage;
        } else if constexpr (std::is_same<T, PlayerComponent>::value) {
            return ComponentType::Player;
        }
    }

    size_t maxEntities;
    size_t entityCount = 0;
    std::vector<uint32_t> entities;
    std::vector<std::bitset<64>> entityComponentMasks;
    std::vector<std::vector<std::shared_ptr<void>>> componentPools;
};

#endif // !__ENTITY_MANAGER_H_
