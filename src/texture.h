#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <memory>

#include "vec2.h"

class Texture {
   public:
    Texture(SDL_Texture* tex, Vec2 dimensions)
        : m_Tex(tex), m_Dimensions(dimensions) {}
    Texture(SDL_Texture* tex, SDL_Surface* surface) : m_Tex(tex) {
        m_Dimensions.x = surface->w;
        m_Dimensions.y = surface->h;
    }
    ~Texture() { SDL_DestroyTexture(m_Tex); }

    Vec2* GetDimensions() { return &m_Dimensions; }
    SDL_Texture* GetTexture() const { return m_Tex; }

    static std::shared_ptr<Texture> Load(SDL_Renderer* renderer, char* path) {
        SDL_Surface* tmpSurface = IMG_Load(path);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        if (tex == NULL) {
            std::cout << "Error: Couldn't load texture from path: '"
                      << path << "'!" << std::endl;
        }

        std::shared_ptr<Texture> output =
            std::make_shared<Texture>(tex, tmpSurface);
        SDL_FreeSurface(tmpSurface);
        return output;
    }

    static std::shared_ptr<Texture> Create(SDL_Renderer* renderer,
                                           SDL_Surface* surface,
                                           bool freeSurface = true) {
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
        if (tex == NULL) {
            std::cout << "Error: Couldn't create texture from surface"
                      << std::endl;
        }

        std::shared_ptr<Texture> output = std::make_shared<Texture>(tex, surface);
        if (freeSurface) SDL_FreeSurface(surface);
        return output;
    }

   private:
    SDL_Texture* m_Tex;
    Vec2 m_Dimensions;
};