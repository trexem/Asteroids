#pragma once
#ifndef __MOVEMENT_SYSTEM_H_
#define __MOVEMENT_SYSTEM_H_

#include "System.h"

class MovementSystem {
public:
    void update(EntityManager* eMgr, double dT);
    void updatePlayer(EntityManager* eMgr, double dT, uint32_t eID);
};


#endif // !__MOVEMENT_SYSTEM_H_