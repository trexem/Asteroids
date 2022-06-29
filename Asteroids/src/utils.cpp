#include "utils.hpp"

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    //If any of the sides from A are outside of B
    if (a.y + a.h <= b.y)
    {
        return false;
    }

    if (a.y >= b.y + b.h)
    {
        return false;
    }

    if (a.x + a.w <= b.x)
    {
        return false;
    }

    if (a.x >= b.x + b.w)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}