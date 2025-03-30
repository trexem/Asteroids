#pragma once
#ifndef __MOVEMENT_SYSTEM_H_
#define __MOVEMENT_SYSTEM_H_

#include "Camera.h"
#include "System.h"

class MovementSystem {
public:
    MovementSystem(Camera* camera) : camera(camera) {}
    void update(EntityManager* eMgr, double dT);
    void updatePlayer(EntityManager* eMgr, double dT, uint32_t eID);
    Camera* camera;
};


#endif // !__MOVEMENT_SYSTEM_H_