#pragma once

#include "System.h"

using CellMap = std::unordered_map<std::pair<int, int>, std::vector<uint32_t>, pair_hash>;

class ProximitySystem : public System {
public:
    void update(EntityManager& eMgr, const double& dT) override;

    CellMap spatialGrid;
    float maxRange = 500.0f;
    float maxRangeSq = maxRange * maxRange;
};
