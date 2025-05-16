#pragma once

#include "System.h"
#include "utils.hpp"

class OrbitSystem : public System {
public:
    OrbitSystem(EntityManager& eM) : eManager(eM) {}
    void update(EntityManager& eMgr, const double& dT) override;
private:
    EntityManager& eManager;
};
