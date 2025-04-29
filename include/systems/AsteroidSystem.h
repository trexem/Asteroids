#pragma once

#include <cmath>
#include <cstdlib>
#include <time.h>

#include "AsteroidAsteroidCollisionMessage.h"
#include "Colors.h"
#include "DestroyAsteroidMessage.h"
#include "ExperienceSpawnMessage.h"
#include "MessageManager.h"
#include "System.h"
#include "timer.hpp"
#include "utils.hpp"

constexpr int MAX_ASTEROIDS = 50;
constexpr float OUTER_RADIUS = 1.0f; // Factor for spawn distance
constexpr float ANGLE_BIAS_STRENGTH = PI; // 90degree bias
constexpr float ASTEROID_BASE_HEALTH = 50.0f;
constexpr float ASTEROID_MAX_SPEED = 150.0f;

class AsteroidSystem : public System {
public:
    AsteroidSystem(EntityManager* eManager, SDL_Renderer* renderer);
    void update(EntityManager* eManager, double timePassed);
    void generateAsteroids(EntityManager* eManager, double timePassed);

private:
    void generateSingleAsteroid(EntityManager* eManager, int lvl);
    FPair generatePosition(EntityManager* eManager);
    void handleDestroyAsteroidMessage(std::shared_ptr<DestroyAsteroidMessage> msg);
    void handleAsteroidAsteroidCollision(std::shared_ptr<AsteroidAsteroidCollisionMessage> msg);

    EntityManager* eManager;
    SDL_Renderer* renderer;
    uint32_t asteroidCount {0};
    std::unordered_set<uint32_t> asteroids;
};
