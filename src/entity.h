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
        rect.w = tex->GetRect()->w;
        rect.h = tex->GetRect()->h;
    }
    Entity(std::shared_ptr<Texture>) : pos(Vec2()), vel(Vec2()), tex(tex) {
        UpdateRect();
        rect.w = tex->GetRect()->w;
        rect.h = tex->GetRect()->h;
    }

    inline const void UpdateRect() {
        rect.x = pos.x;
        rect.y = pos.y;
    }

    inline const void Render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, tex->GetTexture(), NULL, &rect);
    }
};
