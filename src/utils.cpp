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

float getSquaredDistanceBetweenCircles(const FPair& pos1, const float& radius1, const FPair& pos2, const float& radius2) {
    FPair center1 = pos1 + radius1;
    FPair center2 = pos2 + radius2;
    float dx = center2.x - center1.x;
    float dy = center2.y - center1.y;

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

SDL_FPoint getPivotFromRotationPoint(RotationPoint rp, int w, int h) {
    float x = 0.f, y = 0.f;

    // Horizontal
    if (rp & TopLeft || rp & CenterLeft || rp & BottomLeft)
        x = 0.f;
    else if (rp & TopCenter || rp & CenterCenter || rp & BottomCenter)
        x = w / 2.0f;
    else if (rp & TopRight || rp & CenterRight || rp & BottomRight)
        x = float(w);

    // Vertical
    if (rp & TopLeft || rp & TopCenter || rp & TopRight)
        y = 0.f;
    else if (rp & CenterLeft || rp & CenterCenter || rp & CenterRight)
        y = h / 2.0f;
    else if (rp & BottomLeft || rp & BottomCenter || rp & BottomRight)
        y = float(h);

    // Fallback for Default or unknown
    if (rp == RotationPoint::defaultRotation || (x == 0.f && y == 0.f)) {
        x = 0.f;
        y = 0.f;
    }

    return SDL_FPoint{ x, y };
}
