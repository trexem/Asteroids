#pragma once

#include "System.h"

class TouchMessage;

class TouchJoystickSystem : public System {
public:
	TouchJoystickSystem(EntityManager& eMgr);
	void update(EntityManager& eMgr, const double& dT) override;
	void handleTouchMessage(std::shared_ptr<TouchMessage> msg);

private:
	uint32_t joystickId;
	EntityManager& eM;
	TouchJoystickComponent joystick;
	bool isOn{ false };
	float threshold{ 25.0f };
};
