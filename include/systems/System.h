#pragma once
#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "EntityManager.h"

class System {
public:
    virtual ~System() = default;

    virtual void update(EntityManager eMgr, float dT) {}
protected:
    System() = default;
};

#endif // !__SYSTEM_H_