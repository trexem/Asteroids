#pragma once

#include <cmath>
#include <ostream>
#include <SDL3/SDL_rect.h>

class FPair {
public:
    float x, y;
    // Constructors
    FPair() : x(0.0f), y(0.0f) {}
    FPair(float x, float y) : x(x), y(y) {}
    explicit FPair(float s) : x(s), y(s) {}
    FPair(const SDL_FPoint& p) : x(p.x), y(p.y) {}

    // Assignment operators
    FPair& operator=(float s) { x = y = s; return *this; }
    FPair& operator+=(const FPair& rhs) { x += rhs.x; y += rhs.y; return *this; }
    FPair& operator+=(float s) { x += s; y += s; return *this; }
    FPair& operator-=(const FPair& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    FPair& operator-=(float s) { x -= s; y -= s; return *this; }
    FPair& operator*=(float s) { x *= s; y *= s; return *this; }
    FPair& operator/=(float s) { x /= s; y /= s; return *this; }

    // Arithmetic operators
    FPair operator+(const FPair& rhs) const { return FPair(x + rhs.x, y + rhs.y); }
    FPair operator+(float s) const { return FPair(x + s, y + s); }
    FPair operator-(const FPair& rhs) const { return FPair(x - rhs.x, y - rhs.y); }
    FPair operator-(float s) const { return FPair(x - s, y - s); }
    FPair operator*(float s) const { return FPair(x * s, y * s); }
    FPair operator*(const FPair& rhs) const { return FPair(x * rhs.x, y * rhs.y); }
    FPair operator/(float s) const { return FPair(x / s, y / s); }
    FPair operator/(const FPair& rhs) const { return FPair(x / rhs.x, y / rhs.y); }

    // Comparison operators
    bool operator==(const FPair& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const FPair& rhs) const { return !(*this == rhs); }

    // Vector operations
    float length() const { return std::sqrt(x * x + y * y); }
    float lengthSquared() const { return x * x + y * y; }

    FPair& normalize() {
        float len = length();
        if (len > 0) {
            x /= len;
            y /= len;
        }
        return *this;
    }

    FPair normalized() const {
        FPair result(*this);
        return result.normalize();
    }

    float dot(const FPair& rhs) const { return x * rhs.x + y * rhs.y; }
    float cross(const FPair& rhs) const { return x * rhs.y - y * rhs.x; }

    void clamp(float maxLength) {
        float len = length();
        if (len > maxLength) {
            x = (x / len) * maxLength;
            y = (y / len) * maxLength;
        }
    }

    FPair project(const FPair& normal) const {
        float dot = this->dot(normal);
        return normal * dot;
    }
    
    FPair reject(const FPair& normal) const {
        return *this - project(normal);
    }

    ~FPair() = default;
    float angleTowards(FPair p) {
        return atan2(p.x - x, y - p.y);
    }

    // For debugging
    friend std::ostream& operator<<(std::ostream& os, const FPair& fp) {
        return os << "(" << fp.x << ", " << fp.y << ")";
    }
};
