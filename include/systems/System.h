#pragma once

#include "EntityManager.h"

class System {
public:
    virtual ~System() = default;
    virtual void update(EntityManager& eMgr, const double& dT) = 0;
protected:
    System() = default;
};
