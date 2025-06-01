#pragma once

#include "System.h"

struct SystemRecord {
    std::unique_ptr<System> system;
    std::string name;
};

class SystemManager {
public:
    template<typename T, typename... Args>
    T* registerSystem(Args&&... args) {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = system.get();
        std::string typeName = typeid(T).name();
        systems.push_back({std::move(system), typeName});
        return ptr;
    }

    void updateAll(EntityManager& eM, float dt);

    void resetAll();

private:
    std::vector<SystemRecord> systems;
};
