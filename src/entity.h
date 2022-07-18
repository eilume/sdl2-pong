#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "texture.h"
#include "timestate.h"
#include "vec2.h"

struct Entity {
    Vec2 pos;
    Vec2 vel;
    float angle;
    // // TODO: add scale
    float scale;
    SDL_Rect rect;
    std::shared_ptr<Texture> tex;

    Entity()
        : pos(Vec2()), vel(Vec2()), angle(0.0f), scale(1.0f), tex(nullptr) {}
    Entity(Vec2 pos, Vec2 vel, std::shared_ptr<Texture> tex)
        : pos(pos), vel(vel), angle(0.0f), scale(1.0f), tex(tex) {
        UpdateRect();
    }
    Entity(Vec2 pos, Vec2 vel, float angle, float scale,
           std::shared_ptr<Texture> tex)
        : pos(pos), vel(vel), angle(angle), scale(scale), tex(tex) {
        UpdateRect();
    }
    Entity(std::shared_ptr<Texture> tex)
        : pos(Vec2()), vel(Vec2()), angle(0.0f), scale(1.0f), tex(tex) {
        UpdateRect();
    }

    void UpdateRect() {
        rect.x = pos.x;
        rect.y = pos.y;
        Vec2* dimensions = tex->GetDimensions();
        rect.w = dimensions->x * scale;
        rect.h = dimensions->y * scale;
    }

    void Update(Engine::TimeState& timeState) {
        pos += vel * timeState.GetDeltaTime();
    }

    void Render(SDL_Renderer* renderer) {
        constexpr SDL_Point point = {0, 0};
        if (angle == 0.0f) {
            SDL_RenderCopy(renderer, tex->GetTexture(), NULL, &rect);
        } else {
            SDL_RenderCopyEx(renderer, tex->GetTexture(), NULL, &rect, angle,
                             &point, SDL_FLIP_NONE);
        }
    }
};
