#pragma once

#include <array>
#include <bitset>
#include <functional>
#include <unordered_map>
#include <optional>

#include "abilitiesText.h"
#include "Animation.h"
#include "EntityType.h"
#include "FPair.h"
#include "MetaStats.hpp"
#include "texture.hpp"
#include "GUIState.h"

const int SHIP_TOP_SPEED = 500; //TOP speed of ship, must be lower than the speed of shot
const int SHIP_MIN_SPEED = -200; //Min speed in reverse (lower than forward speed)
const int SHIP_TOP_ROTATION_SPEED = 270;
const int SHIP_SPEED = 15; //acceleration, instead of speed
const int SHIP_ROT_SPEED = 5; //acceleration, instead of speed
const float SHIP_BASE_HEALTH = 100.0f; 

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
    Pickup,
    LifeTime,
    Orbit,
    Follow,
    Background,
    Label,
    Button,
    Slider,
    Container,
    GUIState,
    ClickCallback,
    Tooltip,
    NineGrid,
    TouchJoystick,
    ProximityTracker,
    Lightning,

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
    float rotation {0.0f}; //Offset fromTransform rotDegrees
    RotationPoint rotationPoint {RotationPoint::CenterCenter};

    float getBoundingRadius() const {
        return (shape == Shape::Circle) ? radius :
            std::sqrt(width * width + height * height) * 0.5f;
    }
};

struct RenderComponent {
    Texture* texture{nullptr};
    float size{1.0f}; //Percentage, 1 is original size
    bool isStretched {false};
    FPair exactSize{1.0f, 1.0f};
    Uint8 visibility{255};
    bool free {false};
    const SDL_Color* color {nullptr};

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
    float regen {0.0f};
};

struct DamageComponent {
    float damage {1.0f};

    DamageComponent() = default;
    DamageComponent(float d) : damage(d) {}
};

struct PlayerComponent {
    ShipType type;
    int level {1};
    int xpToNextLevel{30};
    int currentXp{0};
    uint8_t ownedWeaponsCount {0};
    uint8_t ownedPassivesCount {0};
    std::bitset<static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)> ownedWeapons;
    std::bitset<static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount)> ownedPassives;
    std::array<double, static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)> weaponCooldowns{};
    std::array<uint8_t, static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)> weaponLevels{};
    std::array<uint8_t, static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount)> passiveLevels{};
    std::array<bool, static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)> isBursting{};
    std::array<double, static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)> burstShotTimers{};
    std::array<int, static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)> shotsRemainingInBurst{};

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
    float extraSize {1.0f};
    float armor {0.0f};
    float healthRegen {0.0f};
};

struct MovementComponent {
    std::bitset<static_cast<size_t>(MoveState::MoveStateCount)> moveState;
};

struct TypeComponent {
    EntityType type;

    TypeComponent(EntityType t) : type(t) {}
};

struct PickupComponent {
    float value {10.0f};
    bool isPickedUp {false};
};

struct AnimationComponent {
    std::unordered_map<Animation, AnimationState> animations;
    Animation current = Animation::Idle;
    float timeAccumulator = 0.0f;
    int currentFrame = 0;
    bool playing = false;
    bool visible = true;
};

struct GUIComponent {
    FPair pos;
    FPair size;
    uint32_t parent = 0; 
};

struct LifeTimeComponent {
    double lifeTime{0.0f};
};

struct OrbitComponent {
    float radius {0.0f};
    double angle {0.0f};
    float rotationSpeed {0.0f};
    uint32_t parentId;
};

struct FollowComponent {
    uint32_t parentId;
    bool isRadial {false};
    FPair offsetPosition {0.0f};
    double offsetAngle {0.0f};
    float radius {0.0f};
};

struct BackgroundComponent {
    float parallaxFactor = 0.5;

    BackgroundComponent(float f) : parallaxFactor(f) {}
};

struct LabelComponent { 
    std::string text {""};
};

struct ButtonComponent {
    std::string label {""};
    bool disabled{false};
};

struct SliderComponent {
    float value{0};
    float min{0};
    float max{0};
};

struct ContainerComponent {
    bool scrollable{false};
};

struct GUIStateComponent {
    GUIState state {GUIState::Idle};
};

struct ClickCallbackComponent {
    std::function<void(uint32_t)> onClick;
};

struct TooltipComponent {
    std::string text = "";
    FPair mousePos = {0, 0};
};

struct NineGridComponent {
    float left {0};
    float right {0};
    float top {0};
    float bottom {0};

};

struct TouchJoystickComponent {
    bool active = false;
    SDL_FingerID fingerID = 0;
    FPair origin;
    FPair current;
    float radius = 80.0f;
};

struct ProximityTrackerComponent {
    std::optional<uint32_t> closest;
    float distance = 0;
    bool hasLifeTime = false;
    double lifeTime = 0.2f;
};

struct LightningComponent {
    FPair sourcePos;
    FPair targetPos;
    std::vector<FPair> path;
};

std::vector<AbilityChoice> getRandomAbilityChoices(const PlayerComponent& player);
std::string getNextUpgradeText(const PlayerComponent& player, const AbilityChoice& ability);
std::string getAbilityTitle(const AbilityChoice& ability);
std::array<FPair, 4> getCorners(const TransformComponent& tComp, const CollisionComponent& cComp);
