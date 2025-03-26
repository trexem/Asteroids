#pragma once

#include <vector>
#include <unordered_map>

#include "abilities.h"
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
    Movement,
    Type,

    Count
};

enum MoveState {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    TURN_LEFT,
    TURN_RIGHT,

    MoveStateCount
};

enum class EntityType {
    Player,
    Asteroid,
    Enemy,
    Shot
};

enum ShipType {
    TANK,
    FREE_MOVE,

};

struct TransformComponent {
    FPair position {0.0f, 0.0f};
    double rotDegrees {0.0f};
};

struct PhysicsComponent {
    float velocity;
    float rotVelocity {0.0f};
    float acceleration;
    float mass {1.0f};
};

struct CollisionComponent {
    float height {1.0f};
    float width {1.0f};
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
    ShipType type;
    std::bitset<static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)> abilities;
    std::unordered_map<ShipAbilities, double> abilityCooldowns;
    std::unordered_map<ShipAbilities, uint> abilityLevels;
};

struct StatsComponent {
    float speed {1.0f};
    float maxSpeed {1.0f};
    float minSpeed {-1.0f};
    float rotationSpeed {1.0f};
    float maxRotationSpeed {1.0f};
    float maxHealth {1.0f};
    float baseDamage {1.0f}; // Multiplier for base damage of abilities****
    float fireSpeed {0.0f}; // goes from 0 to .9999, higher is faster, represents percentage of base weapon cooldown
    float projectileSpeed {1.0f}; // Multiplier for projectile speed
    int projectileCount {0}; // Extra projectiles
};

struct MovementComponent {
    std::bitset<static_cast<size_t>(MoveState::MoveStateCount)> moveState;
};

struct TypeComponent {
    EntityType type;

    TypeComponent(EntityType t) : type(t) {}
};
