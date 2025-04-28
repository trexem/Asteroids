#pragma once

#include "System.h"

class HealthSystem : public System {
public:
    void update(EntityManager* eManager, double dT);
};
