#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "texture.h"

class Font {
   public:
    Font(TTF_Font* font) : m_Font(font) {}
    ~Font() { TTF_CloseFont(m_Font); }

    std::shared_ptr<Texture> TextToTexture(SDL_Renderer* renderer, char* text,
                                           SDL_Color color) {
        SDL_Surface* surface = TTF_RenderText_Solid(m_Font, text, color);
        return Texture::Create(renderer, surface);
    }

    static std::shared_ptr<Font> Load(char* path, int size) {
        TTF_Font* font = TTF_OpenFont(path, size);
        if (font == NULL) {
            std::cout << "Error: Couldn't load font from path: '" << path
                      << "'!" << std::endl;
        }

        return std::make_shared<Font>(font);
    }

   private:
    TTF_Font* m_Font;
};