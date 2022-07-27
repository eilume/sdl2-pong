#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <memory>
#include <string>

#include "timestate.h"

namespace Engine {
typedef void CustomProcessInput(SDL_Event& event);
typedef void CustomUpdate(TimeState& timeState);
typedef void CustomRender();

class Engine {
   public:
    Engine(const std::string& gameName, CustomProcessInput processInput,
           CustomUpdate update, CustomRender render);

    int Setup();
    void Run();
    void Cleanup();

    inline SDL_Window* GetWindow() { return m_Window; }
    inline SDL_Renderer* GetRenderer() { return m_Renderer; }

    inline void QueueQuit() { m_QueueQuit = true; }

#ifdef __EMSCRIPTEN__
    friend void EMSCRIPT_EngineGameLoop(void* enginePtr);
#endif
   private:
    void GameLoop();
    void UpdateTime();

   private:
    CustomProcessInput* f_ProcessInput;
    CustomUpdate* f_Update;
    CustomRender* f_Render;

    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    SDL_Event m_Event;

    std::string m_GameName;
    bool m_QueueQuit = false;

    TimeState m_TimeState;
};
}  // namespace Engine