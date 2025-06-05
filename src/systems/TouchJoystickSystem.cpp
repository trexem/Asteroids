#include "MessageManager.h"
#include "EntityHandle.h"
#include "TouchMessage.h"
#include "KeyboardMessage.h"
#include "TextureManager.h"
#include "Colors.h"
#include "SettingsManager.h"
#include "TouchJoystickSystem.h"
#include "GameStateManager.h"
#include "Log.h"
#include "GUI.h"

TouchJoystickSystem::TouchJoystickSystem(EntityManager& eMgr) : eM(eMgr) {
	MessageManager::instance().subscribe<TouchMessage>(
		[this](std::shared_ptr<TouchMessage> msg) { handleTouchMessage(msg); }
	);
	joystickId = eM.createEntity();
	EntityHandle handle{ joystickId, eM };
	handle.add<TouchJoystickComponent>();
	handle.add<RenderComponent>();
	handle.add<GUIComponent>();
	

	RenderComponent render;
	render.texture = TextureManager::instance().get("gui/touchJoystick");
	render.texture->colorMod(Colors::Health);
	render.visibility = 0;
	render.size = 2.0f;
	GUIComponent gui;
	

	TypeComponent type = EntityType::GUI;
	handle.set(render);
	handle.set(gui);
	handle.set(joystick);
	handle.set(type);
}

void TouchJoystickSystem::handleTouchMessage(std::shared_ptr<TouchMessage> msg) {
	if (!joystick.active && msg->isDown && msg->pos.x <= 0.5f * GUI::screenWidth) {
		joystick.active = true;
		joystick.fingerID = msg->id;
		joystick.origin = msg->pos;
		joystick.current = joystick.origin;
	} else if (joystick.active && msg->id == joystick.fingerID && msg->isDown) {
		joystick.current = msg->pos;
	} else if (joystick.active && msg->id == joystick.fingerID) {
		joystick.active = false;
	}

	if (joystick.active && GameStateManager::instance().getState() == GameState::Playing) {
		RenderComponent render = eM.getComponentData<RenderComponent>(joystickId);
		render.visibility = 255;
		DEBUG_LOG("Joystick visibility is %d", render.visibility);
		eM.setComponentData(joystickId, render);
		GUIComponent gui = eM.getComponentData<GUIComponent>(joystickId);
		gui.pos = joystick.origin - render.texture->getSize();
		eM.setComponentData(joystickId, gui);
	} else {
		RenderComponent render = eM.getComponentData<RenderComponent>(joystickId);
		render.visibility = 0;
		eM.setComponentData(joystickId, render);
	}
}

void TouchJoystickSystem::update(EntityManager& eMgr, const double& dT) {
	if (joystick.active || isOn != joystick.active) {
		FPair rawDelta = joystick.current - joystick.origin;
        DEBUG_LOG("Raw delta (%.2f, %.2f)", rawDelta.x, rawDelta.y);
		auto playerEntities = eMgr.getEntitiesWithComponent(ComponentType::Player);
		for (uint32_t eID : playerEntities) {
			MovementComponent moveComp = eMgr.getComponentData<MovementComponent>(eID);
			moveComp.moveState[MoveState::MOVE_UP] = rawDelta.y < -threshold && joystick.active;
			moveComp.moveState[MoveState::MOVE_DOWN] = rawDelta.y > threshold && joystick.active;
			moveComp.moveState[MoveState::MOVE_LEFT] = rawDelta.x < -threshold && joystick.active;
			moveComp.moveState[MoveState::MOVE_RIGHT] = rawDelta.x > threshold && joystick.active;
			eMgr.setComponentData<MovementComponent>(eID, moveComp);
		}
		isOn = joystick.active;
	}
}
