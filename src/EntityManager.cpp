#include "EntityManager.h"

EntityManager::EntityManager(size_t t_maxEntities) : maxEntities{t_maxEntities} {
    entities.reserve(maxEntities);
    entityComponentMasks.resize(maxEntities);
    componentPools.resize(static_cast<size_t>(ComponentType::Count));
    entityMutexes.resize(maxEntities);
    for (auto& mutex : entityMutexes) {
        mutex = std::make_unique<std::mutex>();
    }
}

EntityManager::~EntityManager() {
    for (uint32_t eID : entities) {
        destroyEntity(eID);
    }
    // Release the memory associated with component pools
    for (auto& componentPool : componentPools) {
        componentPool.clear(); // This will release the unique_ptr and its associated memory
    }
}

uint32_t EntityManager::createEntity() {
    if (entityCount < maxEntities) {
        uint32_t newEntityID = findAvailableEntityID();
        if (destroyedEntities.count(newEntityID) > 0) {
            destroyedEntities.erase(newEntityID);
        }
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
    std::scoped_lock lock(*entityMutexes[entityID], globalMutex);
    if (entityExists(entityID)) {
        // std::cout << "Destroying Entity " << entityID << std::endl;
        freeTexturePtr(entityID);
        entities.erase(std::remove(entities.begin(), entities.end(), entityID), entities.end());
        
        entityComponentMasks[entityID].reset();
        for (size_t i = 0; i < componentPools.size(); i++) {
            if (entityID < componentPools[i].size()) {
                componentPools[i][entityID].reset();  //Properly releases memory
            }
        }
        destroyedEntities.insert(entityID);
        entityCount--;
        // std::cout << "Entity destroyed: " << entityID << std::endl;
    }
}

void EntityManager::addComponent(uint32_t entityID, ComponentType type) {
    std::scoped_lock lock(*entityMutexes[entityID]);
    assert(entityID < maxEntities);
    // Add TypeComponent, if it's not there already, with default type
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

std::mutex& EntityManager::getEntityMutex(uint32_t entityID) {
    assert(entityID < maxEntities);
    return *entityMutexes[entityID];
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
    SDL_Log("Component Pools for Entity %d:", entityID);
    for (size_t typeIdx = 0; typeIdx < static_cast<size_t>(ComponentType::Count); ++typeIdx) {
        auto& pool = componentPools[typeIdx];
        if (entityID < pool.size() && pool[entityID]) {
            SDL_Log("  Component Type %zu  at address %d", typeIdx, pool[entityID].get());
        } else {
            SDL_Log("  Component Type %zu is empty for this entity.", typeIdx);
        }
    }
}

void EntityManager::freeTexturePtr(uint32_t eID) {
    if (entityExists(eID)) {
        if(hasComponent<RenderComponent>(eID)) {
            RenderComponent render = getComponentData<RenderComponent>(eID);
            if (render.free) {
                delete render.texture;
            }
        }
    }
}

void EntityManager::clear() {
    std::vector<uint32_t> toDestroy = entities;
    for (uint32_t eID : toDestroy) {
        destroyEntity(eID);
    }
    toDestroy.clear();
}

bool EntityManager::isDestroyed(uint32_t id) const {
    return destroyedEntities.count(id) > 0;
}

bool EntityManager::isMarkedForDestruction(uint32_t id) {
    std::lock_guard<std::mutex> lock(toDestroyMutex);
    return toDestroy.count(id) > 0;
}

void EntityManager::destroyEntityLater(uint32_t id) {
    std::lock_guard<std::mutex> lock(toDestroyMutex);
    toDestroy.insert(id);
}

void EntityManager::applyPendindDestructions() {
    std::lock_guard<std::mutex> lock(toDestroyMutex);
    for (uint32_t id: toDestroy) {
        destroyEntity(id);
    }
    toDestroy.clear();
}

void EntityManager::clearGameEntities() {
    std::vector<uint32_t> toDestroy = entities;
    for (uint32_t e : toDestroy) {
        if (hasComponent<GUIComponent>(e) || hasComponent<BackgroundComponent>(e)) continue;
        destroyEntity(e);
    }
}
