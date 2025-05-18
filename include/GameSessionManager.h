#pragma once

#include "cstdint"

struct GameSessionStats {
    uint32_t gold = 0;
    uint32_t asteroidsDestroyed = 0;

    void reset() {
        gold = 0;
        asteroidsDestroyed = 0;
    }
};

class GameSessionManager {
public:
    static GameSessionManager& instance();
    void reset();
    GameSessionStats& getStats();

private:
    GameSessionStats stats;
    GameSessionManager() = default;
};
