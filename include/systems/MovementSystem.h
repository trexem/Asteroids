#pragma once

#include "Camera.h"
#include "DestroyAsteroidMessage.h"
#include "MessageManager.h"
#include "System.h"

class MovementSystem {
public:
    MovementSystem(Camera* camera) : camera(camera) {}
    void update(EntityManager* eMgr, double dT);
    void updatePlayer(EntityManager* eMgr, double dT, uint32_t eID);
    Camera* camera;
};
