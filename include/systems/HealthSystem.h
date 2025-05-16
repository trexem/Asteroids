#pragma once

#include "System.h"

class HealthSystem : public System {
public:
    void update(EntityManager& eMgr, const double& dT) override;
};
