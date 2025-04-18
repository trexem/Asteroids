#include <unordered_set>

enum EntityType : uint32_t {
    Player          = 1 << 0, //  1
    Asteroid        = 1 << 1, //  2
    Enemy           = 1 << 2, //  4
    Shot            = 1 << 3, //  8
    GUI             = 1 << 4, // 16
    Experience      = 1 << 5, // 32
    Rocket          = 1 << 6, // 64
    Explosion       = 1 << 7, //128
    Default         = 0
};

class TypesSet {
public:
    inline static constexpr uint32_t PLAYER_EXPERIENCE = EntityType::Player | EntityType::Experience;
    inline static constexpr uint32_t PLAYER_ASTEROID = EntityType::Player | EntityType::Asteroid;
    inline static constexpr uint32_t PLAYER_SHOT = EntityType::Player | EntityType::Shot;
    inline static constexpr uint32_t SHOT_ASTEROID = EntityType::Shot | EntityType::Asteroid;
    inline static constexpr uint32_t ROCKET_ASTEROID = EntityType::Rocket | EntityType::Asteroid;
    inline static constexpr uint32_t EXPLOSION_ASTEROID = EntityType::Explosion | EntityType::Asteroid;
        
    static bool contains(uint32_t mask, EntityType type) {
        return (mask & type) != 0;
    }

    static bool match(uint32_t mask, EntityType typeA, EntityType typeB) {
        return (typeA != typeB) && (mask & typeA) && (mask & typeB);
    }

    static bool sameType(EntityType mask, EntityType typeA, EntityType typeB) {
        return typeA == mask && typeB == mask;
    }
};
