#pragma once
#ifndef __FPAIR_H_
#define __FPAIR_H_

class FPair {
public:
    float x, y;
    FPair(float x, float y) : x(x), y(y) {}
    ~FPair() = default;
};
#endif // !__FPAIR_H_
