#include "TooltipSystem.h"
#include "TextureManager.h"
#include "Fonts.h"
#include "Colors.h"


void TooltipSystem::update(EntityManager& eM, const double& dT) {
    uint32_t hovered = 0;
    for (uint32_t id : eM.getEntitiesWithComponent(ComponentType::GUIState)) {
        const GUIStateComponent* state = eM.getComponentDataPtr<GUIStateComponent>(id);
        if (state && state->state == GUIState::Hovered && eM.hasComponent<TooltipComponent>(id)) {
            hovered = id;
            break;
        }
    }

    if (hovered != lastHovered) {
        destroyTooltip(eM);
        if (hovered != 0) {
            createTooltip(eM, hovered);
        }
        lastHovered = hovered;
    } else if (hovered != 0) { //Update pos for already hovered
        const TooltipComponent& tip = eM.getComponentData<TooltipComponent>(hovered);
        FPair pos = tip.mousePos + 16.0f;
        GUIComponent gui = eM.getComponentData<GUIComponent>(tooltipEntity);
        gui.pos = pos;
        eM.setComponentData(tooltipEntity, gui);
    }
}

void TooltipSystem::createTooltip(EntityManager& eM, uint32_t sourceId) {
    const TooltipComponent& tip = eM.getComponentData<TooltipComponent>(sourceId);
    FPair pos = tip.mousePos + 16.0f;
    Texture* panelTex = TextureManager::instance().get("gui/tooltip");

    //create background
    tooltipEntity = eM.createEntity();
    eM.addComponent(tooltipEntity, ComponentType::Render);
    eM.addComponent(tooltipEntity, ComponentType::GUI);
    eM.addComponent(tooltipEntity, ComponentType::NineGrid);
        
    textTex = Texture(TextureManager::instance().getRenderer());
    textTex.loadMultilineText(tip.text, Colors::White, Fonts::Body, 300);
    FPair size = textTex.getSize() + 32.0f;
        
    GUIComponent gui;
    gui.pos = pos;
    eM.setComponentData(tooltipEntity, gui);

    RenderComponent panel;
    panel.texture = panelTex;
    panel.exactSize = size;
    eM.setComponentData(tooltipEntity, panel);

    NineGridComponent grid {16.0f, 16.0f, 16.0f, 16.0f};
    eM.setComponentData(tooltipEntity, grid);

    // Create label entity
    labelEntity = eM.createEntity();
    eM.addComponents(labelEntity, {ComponentType::Render, ComponentType::GUI});
    
    gui.pos = 16.0f;
    gui.parent = tooltipEntity;
    eM.setComponentData(labelEntity, gui);

    RenderComponent label;
    label.texture = &textTex;
    eM.setComponentData(labelEntity, label);
}

void TooltipSystem::destroyTooltip(EntityManager& eM) {
    if (tooltipEntity != 0) {
        eM.destroyEntityLater(tooltipEntity);
        tooltipEntity = 0;
    }
    if (labelEntity != 0) {
        eM.destroyEntityLater(labelEntity);
        labelEntity = 0;
    }
}
