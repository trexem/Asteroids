#pragma once
#ifndef _UTILS_H_
#define _UTILS_H_

#include <SDL3/SDL.h>

struct Pos
{
	double x;
	double y;
};

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const double PI = 3.14159264;
extern Pos g_screen_center;

bool checkCollision(SDL_Rect a, SDL_Rect b);

#endif // !_UTILS_H_

