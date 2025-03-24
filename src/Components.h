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
    Stats,

    Count
};

struct TransformComponent {
    FPair position {0.0f, 0.0f};
    double rotDegrees {0.0f};
};

struct PhysicsComponent {
    FPair velocity;
    float rotVelocity;
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
    bool shipType {true};
};

struct StatsComponent {
    float speed {1.0f};
    float maxSpeed {1.0f};
    float minSpeed {-1.0f};
    float rotationSpeed {1.0f};
    float maxRotationSpeed {1.0f};
    float maxHealth {1.0f};
    float fireSpeed {1.0f};
};
