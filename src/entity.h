#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "texture.h"
#include "vec2.h"

struct Entity {
    Vec2 pos;
    Vec2 vel;
    SDL_Rect rect;
    std::shared_ptr<Texture> tex;

    Entity() : pos(Vec2()), vel(Vec2()), tex(nullptr) {}
    Entity(Vec2 pos, Vec2 vel, std::shared_ptr<Texture> tex)
        : pos(pos), vel(vel), tex(tex) {
        UpdateRect();
        Vec2* dimensions = tex->GetDimensions();
        rect.w = dimensions->x;
        rect.h = dimensions->y;
    }
    Entity(std::shared_ptr<Texture>) : pos(Vec2()), vel(Vec2()), tex(tex) {
        UpdateRect();
        Vec2* dimensions = tex->GetDimensions();
        rect.w = dimensions->x;
        rect.h = dimensions->y;
    }

    inline const void UpdateRect() {
        rect.x = pos.x;
        rect.y = pos.y;
    }

    inline const void Render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, tex->GetTexture(), NULL, &rect);
    }
};
