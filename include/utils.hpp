#pragma once

#include "FPair.h"

#include <cmath>
#include <iomanip>
#include <SDL3/SDL.h>
#include <sstream>
#include <string>
#include <unordered_set>

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

bool checkCollision(SDL_FRect a, SDL_FRect b);
float getSquaredDistanceBetweenCenters(const SDL_FRect& rect1, const SDL_FRect& rect2);
float calculateCenteredX(int w);
std::string formatTimeMMSS(Uint32 seconds);
