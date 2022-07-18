#pragma once

#include <cmath>

class MathUtils {
   public:
    static float Lerp(float a, float b, float t) { return a + (b - a) * t; }

    static float CustomRange(float zeroToOne, float min, float max) {
        return (zeroToOne * (max - min)) + min;
    }

    static constexpr double RadToDeg = 180 / M_PI;
    static constexpr double DegToRad = M_PI / 180;
};