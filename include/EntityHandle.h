#pragma once

#include "EntityManager.h"

struct EntityHandle {
    uint32_t id;
    EntityManager* eManager;
    
    template<typename T>
    void set(const T& data) { 
        eManager->setComponentData<T>(id, data); 
    }
    
    template<typename T>
    void add() { 
        eManager->addComponent(id, EntityManager::getComponentType<T>()); 
    }
};
