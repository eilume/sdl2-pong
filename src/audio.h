#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#include <iostream>
#include <memory>

class Music {
   public:
    Music(Mix_Music* music) : m_Music(music) {}
    ~Music() { Mix_FreeMusic(m_Music); }

    void Play(int loopCount = 0) { Mix_PlayMusic(m_Music, loopCount); }

    void Stop() {
        // Check if music is currently playing
        if (Mix_PlayingMusic() == 0) return;

        Mix_HaltMusic();
    }

    static std::shared_ptr<Music> Load(char* path) {
        Mix_Music* music = Mix_LoadMUS(path);
        if (music == NULL) {
            std::cout << "Error: Couldn't load music from path: '" << path
                      << "'!" << std::endl;
        }

        return std::make_shared<Music>(music);
    }

   private:
    Mix_Music* m_Music;
};

class SFX {
   public:
    SFX(Mix_Chunk* sfx) : m_Sfx(sfx) {}
    ~SFX() { Mix_FreeChunk(m_Sfx); }

    void Play(int channel, int loopCount = 0) {
        m_Channel = Mix_PlayChannel(channel, m_Sfx, loopCount);
        if (m_Channel == -1) {
            std::cout << "Error: Failed to play sfx!" << std::endl;
        }
    }

    void Stop() {
        if (m_Channel == -1) return;

        // Check if channel currently playing sfx
        if (Mix_GetChunk(m_Channel) != m_Sfx) return;

        Mix_HaltChannel(m_Channel);

        m_Channel = -1;
    }

    static std::shared_ptr<SFX> Load(char* path) {
        Mix_Chunk* sfx = Mix_LoadWAV(path);
        if (sfx == NULL) {
            std::cout << "Error: Couldn't load sfx from path: '" << path << "'!"
                      << std::endl;
        }

        return std::make_shared<SFX>(sfx);
    }

   private:
    Mix_Chunk* m_Sfx;
    int m_Channel;
};