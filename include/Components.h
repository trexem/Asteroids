#pragma once

#include <array>
#include <bitset>
#include <unordered_map>

#include "abilitiesText.h"
#include "Animation.h"
#include "EntityType.h"
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
    Animation,
    GUI,
    Experience,
    LifeTime,
    Orbit,

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

enum ShipType {
    TANK,
    FREE_MOVE,

};

enum class Shape {
    Rectangle, Circle
};

struct TransformComponent {
    FPair position {0.0f, 0.0f};
    double rotDegrees {0.0f};
};

struct PhysicsComponent {
    float velocity{0.0f};
    FPair speed{0.0f, 0.0f};
    bool isSpeedVector{false};
    float rotVelocity {0.0f};
    float acceleration {0.0f};
    float mass {1.0f};
};

struct CollisionComponent {
    Shape shape = Shape::Rectangle;
    FPair position {0.0f, 0.0f}; //Offset from Transform Position
    float height {1.0f};
    float width {1.0f};
    float radius {0.5f};

    float getBoundingRadius() const {
        return (shape == Shape::Circle) ? radius :
            std::sqrt(width*width + height*height)/2.0f;
    }
};

struct RenderComponent {
    Texture* texture{nullptr};
    float size{1.0f};
    int visibility{255};
    bool free {false};

    RenderComponent() = default;
    RenderComponent(SDL_Renderer* renderer, const Surface& surface) {
        texture = new Texture(renderer, surface);
        free = true;
    }
    RenderComponent(Texture* t, float s) : texture(t), size(s) {}
};

struct HealthComponent {
    float health {1.0f};
    float maxHealth {1.0f};
    double explosionCooldown {0.0f};
};

struct DamageComponent {
    float damage {1.0f};

    DamageComponent() = default;
    DamageComponent(float d) : damage(d) {}
};

struct PlayerComponent {
    ShipType type;
    int level {1};
    int xpToNextLevel{100};
    int currentXp{0};
    std::bitset<static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)> abilities;
    std::array<double, static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)> abilityCooldowns{};
    std::array<unsigned int, static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)> abilityLevels{};
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
    float collectionRadius {1.0f};
    float extraSize {0.0f};
};

struct MovementComponent {
    std::bitset<static_cast<size_t>(MoveState::MoveStateCount)> moveState;
};

struct TypeComponent {
    EntityType type;

    TypeComponent(EntityType t) : type(t) {}
};

struct ExperienceComponent {
    float xp {10.0f};
};

struct AnimationComponent {
    std::bitset<static_cast<size_t>(Animation::AnimationCount)> playingAnimation;
    std::array<Texture*, AnimationCount> frames;
    double frameTime{0.0f};
    double elapsedTime{0.0f};
    int currentFrame{0};
    bool visible = true;
};

struct GUIComponent {
    bool clickable {false};
    bool hoverable {false};   
};

struct LifeTimeComponent {
    double lifeTime{0.0f};
};

struct OrbitComponent {
    float radius;
    float angle;
    float rotationSpeed;
    uint32_t parentId;
};

std::vector<ShipAbilities> getRandomAbilityChoices(const PlayerComponent& player);
std::string getNextUpgradeText(const PlayerComponent& player, ShipAbilities ability);
