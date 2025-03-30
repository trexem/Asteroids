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
        // std::cout << "Entity created: " << newEntityID << std::endl;
        for (size_t i = 0; i < static_cast<size_t>(ComponentType::Count); ++i) {
            if (entityComponentMasks[newEntityID][i]) {
                // If this prints something bad is happening. entityComponentMasks for this newEntity should be empty
                std::cout << "Component " << i << " assigned to entity " << newEntityID << std::endl;
            }
        }
        return newEntityID;
    } else {
        return 0; // or error code
    }
}

void EntityManager::destroyEntity(uint32_t entityID) {
    if (entityExists(entityID)) {
        entities.erase(std::remove(entities.begin(), entities.end(), entityID), entities.end());
        
        // std::cout << "Destroying Entity " << entityID << std::endl;
        entityComponentMasks[entityID].reset();
        for (size_t i = 0; i < componentPools.size(); i++) {
            if (entityID < componentPools[i].size()) {
                componentPools[i][entityID].reset();  //Properly releases memory
            }
        }
        entityCount--;
    }
}

void EntityManager::addComponent(uint32_t entityID, ComponentType type) {
    assert(entityID < maxEntities);
    // Add TypeComponent if it's not added with default type
    if (!hasComponent<TypeComponent>(entityID)) {
        entityComponentMasks[entityID][static_cast<size_t>(ComponentType::Type)] = true;
        if (componentPools[static_cast<size_t>(ComponentType::Type)].size() <= entityID) {
            componentPools[static_cast<size_t>(ComponentType::Type)].resize(entityID + 1);
        }
        componentPools[static_cast<size_t>(ComponentType::Type)][entityID] = nullptr;
        TypeComponent type = EntityType::Default;
        setComponentData<TypeComponent>(entityID, type);
    }

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
    for (uint32_t entityID : entities) {
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

void EntityManager::printComponentPool(uint32_t entityID) {
    std::cout << "Component Pools for Entity " << entityID << ":\n";
    for (size_t typeIdx = 0; typeIdx < static_cast<size_t>(ComponentType::Count); ++typeIdx) {
        auto& pool = componentPools[typeIdx];
        if (entityID < pool.size() && pool[entityID]) {
            std::cout << "  Component Type " << typeIdx << " at address " << pool[entityID].get() << std::endl;
        } else {
            std::cout << "  Component Type " << typeIdx << " is empty for this entity.\n";
        }
    }
}
