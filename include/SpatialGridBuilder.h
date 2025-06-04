#pragma once

#include "utils.hpp"
#include "EntityManager.h"

#include <vector>

using CellMap = std::unordered_map<std::pair<int, int>, std::vector<uint32_t>, pair_hash>;

class SpatialGrid {
public:
const int CELL_SIZE = 100;
const float INV_CELL_SIZE = 1.0f / 100.0f;

    static SpatialGrid& instance();
    void build(EntityManager& eMgr);
    const CellMap& getGrid() const;
    std::vector<uint32_t> getNearbyEntities(const FPair& pos, float radius, std::vector<bool>& visited);

private:
    SpatialGrid() = default;
    CellMap spatialGrid;
};
