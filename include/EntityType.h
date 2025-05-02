#pragma once

#include <unordered_set>
#include <unordered_map>
#include <cstdint>
#include <iostream>
#include <string>

enum EntityType : uint32_t {
    Default         = 0,
    Player          = 1 << 0,
    Asteroid        = 1 << 1,
    Enemy           = 1 << 2,
    Shot            = 1 << 3,
    GUI             = 1 << 4,
    Experience      = 1 << 5,
    Rocket          = 1 << 6,
    Explosion       = 1 << 7,
    GravitySaw      = 1 << 8,
    Laser           = 1 << 9,
    Explosive       = 1 << 10,
    Gold            = 1 << 11,
    Background      = 1 << 12,
};

inline const std::unordered_map<EntityType, std::string> EntityTypeNames = {
    {EntityType::Player,    "Player"},
    {EntityType::Asteroid,  "Asteroid"},
    {EntityType::Enemy,     "Enemy"},
    {EntityType::Shot,      "Shot"},
    {EntityType::GUI,       "GUI"},
    {EntityType::Experience,"Experience"},
    {EntityType::Rocket,    "Rocket"},
    {EntityType::Explosion, "Explosion"},
    {EntityType::GravitySaw,"GravitySaw"},
    {EntityType::Laser,     "Laser"},
    {EntityType::Explosive, "Explosive"},
    {EntityType::Gold,      "Gold"},
};

std::string getEntityTypeName(EntityType type);

std::ostream& operator<<(std::ostream& os, EntityType type);

constexpr EntityType operator|(EntityType a, EntityType b) {
    return static_cast<EntityType>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

constexpr bool operator&(EntityType a, EntityType b) {
    return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
}

class TypesSet {
public:
    inline static constexpr uint32_t PLAYER_EXPERIENCE = EntityType::Player | EntityType::Experience;
    inline static constexpr uint32_t PLAYER_ASTEROID = EntityType::Player | EntityType::Asteroid;
    inline static constexpr uint32_t SHOT_ASTEROID = EntityType::Shot | EntityType::Asteroid;
    inline static constexpr uint32_t ROCKET_ASTEROID = EntityType::Rocket | EntityType::Asteroid;
    inline static constexpr uint32_t EXPLOSION_ASTEROID = EntityType::Explosion | EntityType::Asteroid;
    inline static constexpr uint32_t SAW_ASTEROID = EntityType::GravitySaw | EntityType::Asteroid;
    inline static constexpr uint32_t LASER_ASTEROID = EntityType::Laser | EntityType::Asteroid;
    inline static constexpr uint32_t EXPLOSIVE_ASTEROID = EntityType::Explosive | EntityType::Asteroid;
    inline static constexpr uint32_t PROJECTILES = EntityType::Explosion | EntityType::Rocket | EntityType::Shot |
        EntityType::GravitySaw | EntityType::Laser | EntityType::Explosive;
    inline static constexpr uint32_t PLAYER_PROJECTILES = PROJECTILES | EntityType::Player;
    inline static constexpr uint32_t PICKUPS = EntityType::Experience | EntityType::Gold;
    inline static constexpr uint32_t PICKUPS_PROJECTILES = PROJECTILES | PICKUPS;
    inline static constexpr uint32_t ENEMIES = EntityType::Asteroid;
    inline static constexpr uint32_t PICKUPS_ENEMIES = ENEMIES | PICKUPS;
    inline static constexpr uint32_t PICKUPS_PLAYER = EntityType::Player | PICKUPS;
        
    static bool contains(uint32_t mask, EntityType type);

    static bool match(uint32_t mask, EntityType typeA, EntityType typeB);

    static bool sameType(EntityType mask, EntityType typeA, EntityType typeB);

    static bool shouldIgnoreCollision(EntityType a, EntityType b);

    static bool shouldDestroyIfFar(EntityType a);

    static bool shouldPlayerPick(EntityType a, EntityType b);
};
