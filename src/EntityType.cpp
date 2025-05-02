#include "EntityType.h"

std::string getEntityTypeName(EntityType type) {
    if (auto it = EntityTypeNames.find(type); it != EntityTypeNames.end()) {
        return it->second;
    }
    return "Unknown";
}

std::ostream& operator<<(std::ostream& os, EntityType type) {
    os << getEntityTypeName(type); // or getEntityTypePosition(type)
    return os;
}

bool TypesSet::contains(uint32_t mask, EntityType type) {
    return (mask & type) != 0;
}

bool TypesSet::match(uint32_t mask, EntityType typeA, EntityType typeB) {
    return (typeA != typeB) && (mask & typeA) && (mask & typeB);
}

bool TypesSet::sameType(EntityType mask, EntityType typeA, EntityType typeB) {
    return typeA == mask && typeB == mask;
}

bool TypesSet::shouldIgnoreCollision(EntityType a, EntityType b) {
    if ((a & ENEMIES) && (b & ENEMIES)) return false;
    if (((PROJECTILES & a) && (PROJECTILES & b))) return true;
    if (((PLAYER_PROJECTILES & a) && (PLAYER_PROJECTILES & b))) return true;
    if (((PICKUPS_PROJECTILES & a) && (PICKUPS_PROJECTILES & b))) return true;
    if (((PICKUPS & a) && (PICKUPS & b))) return true;
    if (((PICKUPS_ENEMIES & a) && (PICKUPS_ENEMIES & b))) return true;
    return false;
}

bool TypesSet::shouldDestroyIfFar(EntityType a) {
    if (a & PICKUPS) return false;
    if (a & EntityType::Background) return false;
    return true;
}

bool TypesSet::shouldPlayerPick(EntityType a, EntityType b) {
    if (a == b) return false;
    if (a & PICKUPS_PLAYER && b & PICKUPS_PLAYER) return true;
    return false;
}
