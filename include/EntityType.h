#include <unordered_set>

enum EntityType : uint32_t {
    Player          = 1 << 0,
    Asteroid        = 1 << 1,
    Enemy           = 1 << 2,
    Shot            = 1 << 3,
    GUI             = 1 << 4,
    Experience      = 1 << 5,

    Default         = 0
};

class TypesSet {
public:
    inline static constexpr uint32_t PLAYER_EXPERIENCE = EntityType::Player | EntityType::Experience;
    inline static constexpr uint32_t PLAYER_ASTEROID = EntityType::Player | EntityType::Asteroid;
    inline static constexpr uint32_t PLAYER_SHOT = EntityType::Player | EntityType::Shot;
    inline static constexpr uint32_t SHOT_ASTEROID = EntityType::Shot | EntityType::Asteroid;
        
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
