#pragma once

class GUI {
public:
    inline static float screenWidth = 1920.0f;
    inline static float screenHeight = 1080.0f;
    inline static FPair screenCenter = {screenWidth / 2.0f, screenHeight / 2.0f};
    inline static FPair screenTopCenter = {screenWidth / 2.0f, 0};
    inline static float containerWidth = screenWidth * 0.21875f;
    inline static float containerHeight = screenHeight * 0.629f;
    inline static float containerGap = screenWidth * 0.010416f;
    inline static float healthbarWidth = screenWidth * 0.02604166f;
    inline static float healtbarHeight = screenHeight * 0.370370f;

    static void update(FPair newScreenSize) {
        screenWidth = newScreenSize.x;
        screenHeight = newScreenSize.y;
        screenCenter = {screenWidth / 2.0f, screenHeight / 2.0f};
        screenTopCenter = {screenWidth / 2.0f, 0};
        containerWidth = screenWidth * 0.21875f;
        containerHeight = screenHeight * 0.629f;
        containerGap = screenWidth * 0.010416f;
        healthbarWidth = screenWidth * 0.02604166f;
        healtbarHeight = screenHeight * 0.370370f;
    }
};
