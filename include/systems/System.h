#pragma once

#include "EntityManager.h"

class System {
public:
    virtual ~System() = default;

    virtual void update(EntityManager eMgr, float dT) {}
protected:
    System() = default;
};
