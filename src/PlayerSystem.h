#pragma once
#ifndef __PLAYER_SYSTEM_H_
#define __PLAYER_SYSTEM_H_

#include <iostream>

#include "System.h"
#include "MessageManager.h"
#include "KeyboardMessage.h"
#include "EntityManager.h"

class PlayerSystem : public System {
public:
    PlayerSystem(EntityManager* eManager);
    void update();

private:
    void handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg);
    EntityManager* eManager;
};

#endif // !__PLAYER_SYSTEM_H_