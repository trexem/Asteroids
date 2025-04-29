#pragma once

#include <algorithm>
#include <bitset>
#include <cassert>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <vector>

#include "Components.h"
#include "ComponentTraits.h"

class EntityManager {
public:
    EntityManager(size_t t_maxEntities);
    ~EntityManager();
    void clear();
    void clearGameEntities();
    uint32_t createEntity();
    const bool entityExists(uint32_t entityID) const {
        return entityID < maxEntities && entityComponentMasks[entityID].any();
    }
    std::mutex& getEntityMutex(uint32_t entityID);

    void addComponent(uint32_t entityID, ComponentType type);
    void addComponents(uint32_t entityID, const std::vector<ComponentType>& types);
    void removeComponent(uint32_t entityID, ComponentType type);
    void removeComponents(uint32_t entityID, const std::vector<ComponentType>& types);


    template <typename T>
    void setComponentData(uint32_t entityID, const T& componentData) {
        //std::cout << "Setting data for component: " << typeid(T).name() << std::endl;
        static_assert(std::is_standard_layout<T>::value, "Component type must be standard layout.");
        assert(entityID < maxEntities);
        
        constexpr ComponentType type = ComponentTraits<T>::type;
        if (entityComponentMasks[entityID][static_cast<size_t>(type)]) {
            if (entityID < componentPools[static_cast<size_t>(type)].size()) {
                // Create and store a copy of the component data
                componentPools[static_cast<size_t>(type)][entityID] = std::make_unique<T>(componentData);
            }
        }
    }

    template <typename T>
    const T& getComponentData(uint32_t entityID) const {
        constexpr ComponentType type = ComponentTraits<T>::type;
        // std::cout << "Getting component data for component: " << typeid(T).name() 
        // << " for entity: " << entityID << std::endl;
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
    const T& getComponentData(uint32_t entityID) {
        constexpr ComponentType type = ComponentTraits<T>::type;
        // std::cout << "Getting component data for component: " << typeid(T).name() 
        // << " for entity: " << entityID << std::endl;
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
    const T* getComponentDataPtr(uint32_t entityID) const {
        constexpr ComponentType type = ComponentTraits<T>::type;
        if (entityComponentMasks[entityID][static_cast<size_t>(type)]) {
            if (entityID < componentPools[static_cast<size_t>(type)].size()) {
                if (componentPools[static_cast<size_t>(type)][entityID]) {
                    return static_cast<T*>(componentPools[static_cast<size_t>(type)][entityID].get());
                }
            }
        }
        return nullptr; // Return nullptr instead of throwing an exception
    }

    // Non-const overload for modification
    template <typename T>
    T* getComponentDataPtr(uint32_t entityID) {
        constexpr ComponentType type = ComponentTraits<T>::type;
        if (entityComponentMasks[entityID][static_cast<size_t>(type)]) {
            if (entityID < componentPools[static_cast<size_t>(type)].size()) {
                if (componentPools[static_cast<size_t>(type)][entityID]) {
                    return static_cast<T*>(componentPools[static_cast<size_t>(type)][entityID].get());
                }
            }
        }
        return nullptr; // Return nullptr instead of throwing an exception
    }

    template <typename T>
    bool hasComponent(uint32_t entityID) const {
        constexpr ComponentType type = ComponentTraits<T>::type;
        return entityComponentMasks[entityID][static_cast<size_t>(type)];
    }

    bool isDestroyed(uint32_t id) const;
    bool isMarkedForDestruction(uint32_t id);
    void destroyEntityLater(uint32_t id);
    void applyPendindDestructions();


    std::vector<uint32_t> getEntitiesWithComponent(ComponentType type);
    size_t maxEntities;
    std::vector<std::bitset<64>> entityComponentMasks;

    template <typename T>
    static constexpr ComponentType getComponentType() {
        return ComponentTraits<T>::type;
    }
private:
    uint32_t findAvailableEntityID();
    void destroyEntity(uint32_t entityID);
    void printComponentPool(uint32_t entityID);
    void freeTexturePtr(uint32_t eID);
    
    size_t entityCount = 0;
    std::vector<uint32_t> entities;
    std::unordered_set<uint32_t> destroyedEntities;
    std::unordered_set<uint32_t> toDestroy;
    std::vector<std::unique_ptr<std::mutex>> entityMutexes;
    std::mutex globalMutex;
    std::mutex toDestroyMutex;
    
    std::vector<std::vector<std::shared_ptr<void>>> componentPools;
};
