#pragma once

#include <SDL_image.h>

#include "vec2.h"

struct Entity {
    Vec2 pos;
    Vec2 vel;
    SDL_Rect rect;
    SDL_Texture* tex;

    Entity() : pos(Vec2()), vel(Vec2()), tex(nullptr) {}
    Entity(Vec2 position, Vec2 velocity, SDL_Texture* texture,
           Vec2 texDimensions)
        : pos(position), vel(velocity), tex(texture) {
        rect.x = position.x;
        rect.y = position.y;
        rect.w = texDimensions.x;
        rect.h = texDimensions.y;
    }
};
