#pragma once

#include "EntityHandle.h"
#include "System.h"

class TooltipSystem : public System {
public:
    void update(EntityManager& eM, const double& dT) override;
private:
    uint32_t tooltipEntity = 0;
    uint32_t labelEntity = 0;
    uint32_t lastHovered = 0;
    Texture textTex;

    void createTooltip(EntityManager& eM, uint32_t sourceId);
    void destroyTooltip(EntityManager& eM);
};
