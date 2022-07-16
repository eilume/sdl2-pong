#pragma once

#include <cmath>

struct Vec2 {
    float x;
    float y;

    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2(const Vec2& other) : x(other.x), y(other.y) {}

    // TODO: add operator+= and operator*=
    Vec2 operator+(Vec2 other) { return Vec2(x + other.x, y + other.y); }
    Vec2 operator+(float other) { return Vec2(x + other, y + other); }

    Vec2 operator*(float other) { return Vec2(x * other, y * other); }

    float Magnitude() { return sqrt(x * x + y * y); }
};

struct BVec2 {
    bool x;
    bool y;

    BVec2() : x(false), y(false) {}
    BVec2(bool x, bool y) : x(x), y(y) {}

    bool Or() { return x || y; }
    bool And() { return x && y; }
};
