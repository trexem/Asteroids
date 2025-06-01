#include "SystemManager.h"
#include "Log.h"

#include <chrono>

void SystemManager::updateAll(EntityManager& eM, float dt) {
    for (auto& system : systems) {
        auto start = std::chrono::high_resolution_clock::now();
        system.system->update(eM, dt);
        auto end = std::chrono::high_resolution_clock::now();
        DEBUG_LOG("[%s] update time: %lld us", system.name.c_str(), std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }
    DEBUG_LOG("Finished updating Systems");
}

void SystemManager::resetAll() {
    for (auto& system : systems) {
        system.system.reset();
    }
}