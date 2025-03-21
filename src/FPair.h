#pragma once
#ifndef __FPAIR_H_
#define __FPAIR_H_

class FPair {
public:
    FPair() : x(0.0f), y(0.0f) {}
    FPair(float x, float y) : x(x), y(y) {}
    ~FPair() = default;
    float x, y;
};
#endif // !__FPAIR_H_
