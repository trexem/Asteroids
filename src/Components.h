#pragma once

#include <vector>

#include "FPair.h"

enum class ComponentType {
    Transform,
    Physics,
    Collision,
    Render,
    Image,

    Count
};

struct TransformComponent {
    FPair Position{0.0f, 0.0f};
    double rot_degrees {0.0f};
};

struct PhysicsComponent {
    FPair velocity;
    FPair acceleration;
    float mass{1.0f};
};

struct CollisionComponent {

};

struct RenderComponent {

};

struct ImageComponent {
    /* data */
};
