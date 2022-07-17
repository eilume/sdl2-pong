#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <memory>

#include "vec2.h"

class Texture {
   public:
    Texture(SDL_Texture* tex, SDL_Surface* surface) : tex(tex) {
        rect.x = 0;
        rect.y = 0;
        rect.h = surface->h;
        rect.w = surface->w;
    }
    ~Texture() { SDL_DestroyTexture(tex); }

   public:
    SDL_Rect* GetRect() { return &rect; }
    SDL_Texture* GetTexture() const { return tex; }

    static std::shared_ptr<Texture> Load(SDL_Renderer* renderer, char* path) {
        SDL_Surface* tmpSurface = IMG_Load(path);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
        return std::make_shared<Texture>(tex, tmpSurface);
    }

   private:
    SDL_Texture* tex;
    SDL_Rect rect;
};