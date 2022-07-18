#pragma once

#include <cmath>

#include "mathutils.h"

struct Vec2 {
    float x;
    float y;

    Vec2() : x(0.0f), y(0.0f) {}
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

    void operator*=(Vec2 other) {
        x *= other.x;
        y *= other.y;
    }
    void operator*=(float other) {
        x *= other;
        y *= other;
    }

    Vec2 operator/(Vec2 other) const { return Vec2(x / other.x, y / other.y); }
    Vec2 operator/(float other) const { return Vec2(x / other, y / other); }

    void operator/=(Vec2 other) {
        x /= other.x;
        y /= other.y;
    }
    void operator/=(float other) {
        x /= other;
        y /= other;
    }

    float Magnitude() { return sqrt(x * x + y * y); }

    Vec2 Normalized() {
        float mag = Magnitude();
        return mag > 0 ? Vec2(x / mag, y / mag) : Vec2();
    }

    Vec2 RotateByRads(float radians) {
        float sin = sinf(radians);
        float cos = cosf(radians);

        return Vec2((cos * x) - (sin * y), (sin * x) + (cos * y));
    }

    Vec2 RotateByDeg(float degrees) {
        return RotateByRads(degrees * MathUtils::DegToRad);
    }

    float GetAngleRads(Vec2 baseVector = Vec2()) {
        Vec2 baseNorm = baseVector.Normalized();
        Vec2 thisNorm = Normalized();

        return atan2(thisNorm.y - baseNorm.y, thisNorm.x - baseNorm.x);
    }

    float GetAngleDeg(Vec2 baseVector = Vec2()) {
        return GetAngleRads() * MathUtils::RadToDeg;
    }

    static Vec2 Lerp(Vec2 a, Vec2 b, float t) { return a + (b - a) * t; }
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
