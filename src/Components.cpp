#include "Components.h"


std::array<FPair, 4> getCorners(const TransformComponent& tComp, const CollisionComponent& cComp) {
    const float rad = (tComp.rotDegrees + cComp.rotation) * DEG2RAD;
    const float c = cosf(rad);
    const float s = sinf(rad);
    float w = cComp.shape == Shape::Rectangle ? cComp.width : cComp.radius;
    float h = cComp.shape == Shape::Rectangle ? cComp.height : cComp.radius;
    FPair pivot = getPivotFromRotationPoint(cComp.rotationPoint, w, h);
    const FPair worldPivot = {
        tComp.position.x + cComp.position.x + pivot.x,
        tComp.position.y + cComp.position.y + pivot.y,
    };
    const std::array<FPair, 4> localCorners = {
        FPair{-pivot.x, -pivot.y},
        FPair{cComp.width - pivot.x, -pivot.y},
        FPair{cComp.width - pivot.x, cComp.height - pivot.y},
        FPair{-pivot.x, cComp.height - pivot.y}
    };
    std::array<FPair, 4> worldCorners;
    for (int i = 0; i < 4; i++) {
        worldCorners[i] = {
            worldPivot.x + localCorners[i].x * c - localCorners[i].y * s,
            worldPivot.y + localCorners[i].x * s + localCorners[i].y * c
        };
    }
    return worldCorners;
}
