#include "SpatialGridBuilder.h"

#include <chrono>

SpatialGrid& SpatialGrid::instance() {
    static SpatialGrid instance;
    return instance;
}

void SpatialGrid::build(EntityManager& eMgr) {
    spatialGrid.clear();
    
    std::vector<uint32_t> physicsEntities = eMgr.getEntitiesWithComponent(ComponentType::Physics);
    // std::cout << "PhysicsEntities count: " << physicsEntities.size() << std::endl;
    for (uint32_t entity : physicsEntities) {
        TransformComponent* trans = eMgr.getComponentDataPtr<TransformComponent>(entity);
        CollisionComponent* col = eMgr.getComponentDataPtr<CollisionComponent>(entity);
        if (!trans || !col) continue;

        auto corners = getCorners(*trans, *col);

        // Find the AABB that encloses the rotated OBB
        float minX = std::min({corners[0].x, corners[1].x, corners[2].x, corners[3].x});
        float maxX = std::max({corners[0].x, corners[1].x, corners[2].x, corners[3].x});
        float minY = std::min({corners[0].y, corners[1].y, corners[2].y, corners[3].y});
        float maxY = std::max({corners[0].y, corners[1].y, corners[2].y, corners[3].y});

        int minCellX = static_cast<int>(minX) / CELL_SIZE;
        int minCellY = static_cast<int>(minY) / CELL_SIZE;
        int maxCellX = static_cast<int>(maxX) / CELL_SIZE;
        int maxCellY = static_cast<int>(maxY) / CELL_SIZE;

        for (int x = minCellX; x <= maxCellX; ++x) {
            for (int y = minCellY; y <= maxCellY; ++y) {
                // std::cout << "adding " << entity << " to grid: " << x << ", " << y << std::endl;
                spatialGrid[{x, y}].push_back(entity);
            }
        }
    }
}

const CellMap& SpatialGrid::getGrid() const {
    return spatialGrid;
}

std::vector<uint32_t> SpatialGrid::getNearbyEntities(const FPair& pos, float radius, std::vector<bool>& visited) {
    using namespace std::chrono;
    auto t0 = high_resolution_clock::now();

    std::vector<uint32_t> result;
    result.reserve(32);

    auto t1 = high_resolution_clock::now();
    int range = static_cast<int>(std::ceil(radius / CELL_SIZE));
    int cellCount = (2 * range + 1) * (2 * range + 1);
    printf("range: %d cells: %d\n", range, cellCount);
    int cellX = static_cast<int>(pos.x * INV_CELL_SIZE);
    int cellY = static_cast<int>(pos.y * INV_CELL_SIZE);
    auto t2 = high_resolution_clock::now();

    int lookups = 0, inserts = 0;

    for (int dx = -range; dx <= range; ++dx) {
        for (int dy = -range; dy <= range; ++dy) {
            auto it = spatialGrid.find({cellX + dx, cellY + dy});
            lookups++;
            if (it != spatialGrid.end()) {
                for (uint32_t id : it->second) {
                    if (!visited[id]) {
                        visited[id] = true;
                        result.push_back(id);
                        inserts++;
                    }
                }
            }
        }
    }

    auto t3 = high_resolution_clock::now();
    for (uint32_t id : result) visited[id] = false;
    auto t4 = high_resolution_clock::now();

    auto total = duration_cast<microseconds>(t4 - t0).count();
    auto alloc = duration_cast<microseconds>(t1 - t0).count();
    auto calc = duration_cast<microseconds>(t2 - t1).count();
    auto query = duration_cast<microseconds>(t3 - t2).count();
    auto reset = duration_cast<microseconds>(t4 - t3).count();

    printf("Grid: %dus [alloc %dus, calc %dus, lookup %dus, reset %dus] | lookups: %d, results: %zu, inserts: %d\n",
        total, alloc, calc, query, reset, lookups, result.size(), inserts);

    return result;
}
