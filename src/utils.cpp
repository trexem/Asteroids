#include "utils.hpp"

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

FPair calculateCenters(float x, float y, float w, float h) {
    return FPair(x + w / 2, y + h / 2);
}

std::string formatTimeMMSS(Uint32 seconds) {
    Uint32 minutes = seconds / 60;
    Uint32 s = seconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << s;

    return oss.str();
}
