#include "EntityManager.h"

EntityManager::EntityManager(size_t t_maxEntities) : maxEntities{t_maxEntities} {
    entities.reserve(maxEntities);
    entityComponentMasks.resize(maxEntities);
    componentPools.resize(static_cast<size_t>(ComponentType::Count));
}

EntityManager::~EntityManager() {
    // Release the memory associated with component pools
    for (auto& componentPool : componentPools) {
        componentPool.clear(); // This will release the unique_ptr and its associated memory
    }
}

uint32_t EntityManager::createEntity() {
    if (entityCount < maxEntities) {
        uint32_t newEntityID = findAvailableEntityID();
        entities.push_back(newEntityID);
        entityCount++;
        std::cout << "Entity created: " << newEntityID << std::endl;
        return newEntityID;
    } else {
        return 0; // or error code
    }
}

void EntityManager::destroyEntity(uint32_t entityID) {
    if (entityExists(entityID)) {
        entities.erase(std::remove(entities.begin(), entities.end(), entityID), entities.end());
        
        entityComponentMasks[entityID].reset();

        for (size_t i = 0; i < componentPools.size(); i++) {
            if (entityID < componentPools[i].size()) {
                componentPools[i][entityID].reset();  //Properly releases memory
            }
        }
        entityCount--;
        std::cout << "Entity destroyed: " << entityID << std::endl;
    }
}

void EntityManager::addComponent(uint32_t entityID, ComponentType type) {
    assert(entityID < maxEntities);
    // Add TransformComponent if renderComponent is being added
    if (type == ComponentType::Render && !hasComponent<TransformComponent>(entityID)) {
        entityComponentMasks[entityID][static_cast<size_t>(ComponentType::Transform)] = true;
        if (componentPools[static_cast<size_t>(ComponentType::Transform)].size() <= entityID) {
            componentPools[static_cast<size_t>(ComponentType::Transform)].resize(entityID + 1);
        }
        componentPools[static_cast<size_t>(ComponentType::Transform)][entityID] = nullptr;
    }

    // Add statsComponent if physicsComponent is being added
    if (type == ComponentType::Physics && !hasComponent<StatsComponent>(entityID)) {
        entityComponentMasks[entityID][static_cast<size_t>(ComponentType::Stats)] = true;
        if (componentPools[static_cast<size_t>(ComponentType::Stats)].size() <= entityID) {
            componentPools[static_cast<size_t>(ComponentType::Stats)].resize(entityID + 1);
        }
        componentPools[static_cast<size_t>(ComponentType::Stats)][entityID] = nullptr;
    }

    entityComponentMasks[entityID][static_cast<size_t>(type)] = true;
    if (componentPools[static_cast<size_t>(type)].size() <= entityID) {
        componentPools[static_cast<size_t>(type)].resize(entityID + 1);
    }
    componentPools[static_cast<size_t>(type)][entityID] = nullptr;
}

void EntityManager::addComponents(uint32_t entityID, const std::vector<ComponentType>& types) {
    assert(entityID < maxEntities);
    for (const ComponentType type : types) {
        addComponent(entityID, type);
    }
}

void EntityManager::removeComponent(uint32_t entityID, ComponentType type) {
    assert(entityID < maxEntities);
    if (entityComponentMasks[entityID][static_cast<size_t>(type)]) {
        entityComponentMasks[entityID][static_cast<size_t>(type)] = false;

        if (entityID < componentPools[static_cast<size_t>(type)].size()) {
            // Clear the component data for the specified entity
            componentPools[static_cast<size_t>(type)][entityID].reset();
        }
    }
}

void EntityManager::removeComponents(uint32_t entityID, const std::vector<ComponentType>& types) {
    assert(entityID < maxEntities);
    for (const ComponentType type : types) {
        removeComponent(entityID, type);
    }
}

std::vector<uint32_t> EntityManager::getEntitiesWithComponent(ComponentType type) {
    std::vector<uint32_t> entitiesWithComponent;
    for (uint32_t entityID = 0; entityID < entityCount; entityID++) {
        if (entityComponentMasks[entityID][static_cast<size_t>(type)]) {
            entitiesWithComponent.push_back(entityID);
        }
    }
    return entitiesWithComponent;
}

uint32_t EntityManager::findAvailableEntityID() {
    for (uint32_t i = 0; i < maxEntities; i++) {
        if (!entityComponentMasks[i].any()) {
            return i;
        }
    }
    return 0;
}
