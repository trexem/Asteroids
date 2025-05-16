#pragma once

#include "System.h"

class SystemManager {
public:
    template<typename T, typename... Args>
    T* registerSystem(Args&&... args) {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = system.get();
        systems.emplace_back(std::move(system));
        return ptr;
    }

    void updateAll(EntityManager& eM, float dt) {
        for (auto& system : systems) {
            system->update(eM, dt);
        }
    }

    void resetAll() {
        for (auto& system : systems) {
            system.reset();
        }
    }

private:
    std::vector<std::unique_ptr<System>> systems;
};
