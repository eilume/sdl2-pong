#pragma once

#include <cmath>

struct Vec2 {
    float x;
    float y;

    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2(const Vec2& other) : x(other.x), y(other.y) {}

    inline bool IsZero() const { return x == 0 && y == 0; }

    bool operator==(Vec2 other) const { return x == other.x && y == other.y; }
    bool operator!=(Vec2 other) const { return x != other.x || y != other.y; }

    Vec2 operator+(Vec2 other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator+(float other) const { return Vec2(x + other, y + other); }

    void operator+=(Vec2 other) {
        x += other.x;
        y += other.y;
    }
    void operator+=(float other) {
        x += other;
        y += other;
    }

    Vec2 operator-(Vec2 other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator-(float other) const { return Vec2(x - other, y - other); }

    void operator-=(Vec2 other) {
        x -= other.x;
        y -= other.y;
    }
    void operator-=(float other) {
        x -= other;
        y -= other;
    }

    Vec2 operator*(Vec2 other) const { return Vec2(x * other.x, y * other.y); }
    Vec2 operator*(float other) const { return Vec2(x * other, y * other); }

    Vec2 operator*=(Vec2 other) {
        x *= other.x;
        y *= other.y;
    }
    Vec2 operator*=(float other) {
        x *= other;
        y *= other;
    }

    Vec2 operator/(Vec2 other) const { return Vec2(x / other.x, y / other.y); }
    Vec2 operator/(float other) const { return Vec2(x / other, y / other); }

    Vec2 operator/=(Vec2 other) {
        x /= other.x;
        y /= other.y;
    }
    Vec2 operator/=(float other) {
        x /= other;
        y /= other;
    }

    float Magnitude() { return sqrt(x * x + y * y); }
};

struct BVec2 {
    bool x;
    bool y;

    BVec2() : x(false), y(false) {}
    BVec2(bool x, bool y) : x(x), y(y) {}

    inline bool Or() const { return x || y; }
    inline bool And() const { return x && y; }

    inline bool True() const { return And(); }
    inline bool False() const { return !And(); }

    bool operator==(Vec2 other) const { return x == other.x && y == other.y; }
    bool operator!=(Vec2 other) const { return x != other.x || y != other.y; }
};
