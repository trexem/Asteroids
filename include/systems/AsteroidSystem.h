#pragma once
#ifndef __ASTEROID_SYSTEM_H_
#define __ASTEROID_SYSTEM_H_

#include <cmath>
#include <cstdlib>
#include <time.h>

#include "System.h"
#include "timer.hpp"
#include "utils.hpp"

constexpr int MAX_ASTEROIDS = 30;
constexpr float OUTER_RADIUS = 1.0f; // Factor for spawn distance
constexpr float ANGLE_BIAS_STRENGTH = PI; // 90degree bias

class AsteroidSystem : public System {
public:
    AsteroidSystem(SDL_Renderer* renderer) : renderer(renderer) {}
    void update(EntityManager* eManager, double timePassed);
    void generateAsteroids(EntityManager* eManager, double timePassed);

private:
    void generateSingleAsteroid(EntityManager* eManager, int lvl);
    FPair generatePosition(EntityManager* eManager);
    void updateAsteroidCount(EntityManager* eManager);

    SDL_Renderer* renderer;
    uint32_t asteroidCount {0};
};

#endif // ! __ASTEROID_SYSTEM_H_
