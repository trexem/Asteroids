#pragma once

#include <vector>

#include "FPair.h"
#include "texture.hpp"

enum class ComponentType {
    Transform,
    Physics,
    Collision,
    Render,
    Health,
    Damage,
    Player,

    Count
};

struct TransformComponent {
    FPair Position {0.0f, 0.0f};
    double rot_degrees {0.0f};
};

struct PhysicsComponent {
    FPair velocity;
    FPair acceleration;
    float mass {1.0f};
};

struct CollisionComponent {

};

struct RenderComponent {
    Texture* texture;
};

struct HealthComponent {
    float health {1.0f};
    float maxHealth {1.0f};
};

struct DamageComponent {
    float damage {1.0f};
};

struct PlayerComponent {
};
