#include "utils.hpp"

bool checkCollision(SDL_FRect a, SDL_FRect b) {
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

float getSquaredDistanceBetweenCenters(const SDL_FRect& rect1, const SDL_FRect& rect2) {
    float centerX1 = rect1.x + rect1.w / 2.0f;
    float centerY1 = rect1.y + rect1.h / 2.0f;

    float centerX2 = rect2.x + rect2.w / 2.0f;
    float centerY2 = rect2.y + rect2.h / 2.0f;

    float dx = centerX2 - centerX1;
    float dy = centerY2 - centerY1;

    return dx * dx + dy * dy;
}

float calculateCenteredX(int w) {
    return SCREEN_CENTER.x - w / 2;
}
