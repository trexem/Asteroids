#pragma once

#include "System.h"

class MouseMotionMessage;
class ClickMessage;

class GUIInteractionSystem : public System {
public:
    GUIInteractionSystem(EntityManager& eM);
    void update(EntityManager& eMgr, const double& dT) override;
private:
    void handleMouseMotionMessage(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClickMessage(std::shared_ptr<ClickMessage> msg);
    EntityManager& eM;
};
