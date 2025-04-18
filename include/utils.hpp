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

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const FPair SCREEN_CENTER = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
const FPair SCREEN_TOP_CENTER = { SCREEN_CENTER.x, 0 };
constexpr double PI = 3.14159264;
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
