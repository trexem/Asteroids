#pragma once

#include "ExplodeMessage.h"
#include "MessageManager.h"
#include "System.h"

class LifeTimeSystem : public System {
public:
    LifeTimeSystem(EntityManager& eMgr);
    void update(EntityManager& eMgr, const double& dT) override;
private:
    EntityManager& eManager;
};
