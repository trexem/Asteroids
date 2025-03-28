#pragma once
#ifndef _UTILS_H_
#define _UTILS_H_

#include "FPair.h"

#include <SDL3/SDL.h>
#include <cmath>

struct Pos
{
	double x;
	double y;
};

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const FPair SCREEN_CENTER = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
const FPair SCREEN_TOP_CENTER = { SCREEN_CENTER.x, 0 };
const double PI = 3.14159264;
extern Pos g_screen_center;

bool checkCollision(SDL_FRect a, SDL_FRect b);

#endif // !_UTILS_H_

