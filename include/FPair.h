#pragma once
#ifndef __FPAIR_H_
#define __FPAIR_H_

class FPair {
public:
    FPair() : x(0.0f), y(0.0f) {}
    FPair(float x, float y) : x(x), y(y) {}
    FPair operator=(const float s) { return FPair(s, s); }
    FPair operator+=(const FPair pair) { 
        x += pair.x;
        y += pair.y;
        return FPair(x, y);
    }
    FPair operator+=(const float s) { 
        x += s;
        y += s;
        return FPair(x, y);
    }
    FPair operator*(const float s) {
        x *= s;
        y *= s;
        return FPair(x, y);
    }
    FPair operator*=(const float s) {
        x *= s;
        y *= s;
        return FPair(x, y);
    }
    ~FPair() = default;
    float x, y;
};
#endif // !__FPAIR_H_
