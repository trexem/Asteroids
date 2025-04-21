#pragma once

#include "FPair.h"

#include <cmath>
#include <iomanip>
#include <SDL3/SDL.h>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

struct Pos
{
	double x;
	double y;
};

constexpr float SCREEN_WIDTH = 1920;
constexpr float SCREEN_HEIGHT = 1080;
const FPair SCREEN_CENTER = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
const FPair SCREEN_TOP_CENTER = { SCREEN_CENTER.x, 0 };
constexpr float PI = 3.141592653589793f;
constexpr float TAU = 2.0f * PI;
constexpr float HALF_PI = PI / 2.0f;
constexpr float DEG2RAD = PI / 180.0f;
constexpr float RAD2DEG = 180.0f / PI;
extern Pos g_screen_center;

float getSquaredDistanceBetweenCenters(const SDL_FRect& rect1, const SDL_FRect& rect2);
float calculateCenteredX(int w);
FPair calculateCenters(float x, float y, float w, float h);
std::string formatTimeMMSS(Uint32 seconds);

struct pair_hash {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);   // Hash the first element (cellX)
        auto h2 = std::hash<T2>{}(p.second);  // Hash the second element (cellY)
        return h1 ^ h2; // Combine them (XOR works fine for most things)
    }
};
