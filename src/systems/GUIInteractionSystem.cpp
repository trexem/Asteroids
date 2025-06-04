#include "ClickMessage.h"
#include "MouseMotionMessage.h"
#include "MessageManager.h"
#include "GUIInteractionSystem.h"

GUIInteractionSystem::GUIInteractionSystem(EntityManager& eM) : eM(eM) {
    MessageManager::instance().subscribe<MouseMotionMessage>(
        [this](std::shared_ptr<MouseMotionMessage> msg) { handleMouseMotionMessage(msg); }
    );

    MessageManager::instance().subscribe<ClickMessage>(
        [this](std::shared_ptr<ClickMessage> msg) { handleMouseClickMessage(msg); }
    );
}

void GUIInteractionSystem::update(EntityManager& eMgr, const double& dT) {
}

void GUIInteractionSystem::handleMouseMotionMessage(std::shared_ptr<MouseMotionMessage> msg) {
    for (uint32_t e : eM.getEntitiesWithComponent(ComponentType::GUIState)) {
        auto gui = eM.getComponentDataPtr<GUIComponent>(e);
        auto state = eM.getComponentData<GUIStateComponent>(e);
        if (state.state == GUIState::Disabled || state.state == GUIState::Selected) continue;
        SDL_FRect bounds {gui->pos.x, gui->pos.y, gui->size.x, gui->size.y};
        if (msg->mousePos.x >= bounds.x && msg->mousePos.x <= bounds.x + bounds.w &&
            msg->mousePos.y >= bounds.y && msg->mousePos.y <= bounds.y + bounds.h) {
            if (state.state == GUIState::Idle) {
                state.state = GUIState::Hovered;
            }
            if (state.state == GUIState::Hovered && eM.hasComponent<TooltipComponent>(e)) {
                auto tip = eM.getComponentData<TooltipComponent>(e);
                tip.mousePos = msg->mousePos;
                eM.setComponentData(e, tip);
            }
        } else {
            if (state.state != GUIState::Pressed) {
                state.state = GUIState::Idle;
            }
        }
        eM.setComponentData(e, state);
    }
}

void GUIInteractionSystem::handleMouseClickMessage(std::shared_ptr<ClickMessage> msg) {
    for (uint32_t e : eM.getEntitiesWithComponent(ComponentType::GUIState)) {
        auto gui = eM.getComponentDataPtr<GUIComponent>(e);
        auto state = eM.getComponentData<GUIStateComponent>(e);
        if (state.state == GUIState::Disabled || state.state == GUIState::Selected) continue;
        SDL_FRect bounds {gui->pos.x, gui->pos.y, gui->size.x, gui->size.y};
        bool inside = msg->mousePos.x >= bounds.x && msg->mousePos.x <= bounds.x + bounds.w &&
                    msg->mousePos.y >= bounds.y && msg->mousePos.y <= bounds.y + bounds.h;
        if (msg->isDown) {
            if (inside) {
                state.state = GUIState::Pressed;
            }
        } else {
            if (inside) {
                state.state = GUIState::Clicked;
                if (eM.hasComponent<ClickCallbackComponent>(e)) {
                    eM.getComponentDataPtr<ClickCallbackComponent>(e)->onClick(e);
                }
                state.state = (inside ? GUIState::Hovered : GUIState::Idle);
            } else if (state.state == GUIState::Pressed) { //Not inside but was pressed
                state.state = GUIState::Idle;
            }
        }
        eM.setComponentData(e, state);
    }
}
