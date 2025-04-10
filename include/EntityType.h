#include <unordered_set>

enum class EntityType {
    Player,
    Asteroid,
    Enemy,
    Shot,
    GUI,
    Experience,

    Default
};

class TypesSet {
    public:
    inline static const std::unordered_set<EntityType> PLAYER_EXPERIENCE = {EntityType::Player, EntityType::Experience};
    inline static const std::unordered_set<EntityType> PLAYER_ASTEROID = {EntityType::Player, EntityType::Asteroid};
    inline static const std::unordered_set<EntityType> PLAYER_SHOT = {EntityType::Player, EntityType::Shot};
    inline static const std::unordered_set<EntityType> SHOT_SHOT = {EntityType::Shot, EntityType::Shot};
    inline static const std::unordered_set<EntityType> SHOT_ASTEROID = {EntityType::Shot, EntityType::Asteroid};
        
        static bool contains(const std::unordered_set<EntityType>& set, EntityType type) {
            return set.find(type) != set.end();
        }
};
