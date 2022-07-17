#include "engine.h"

#include "config.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <iostream>

namespace Engine {
Engine::Engine(const std::string& gameName, CustomProcessInput* processInput,
               CustomUpdate* update, CustomRender* render)
    : m_GameName(gameName),
      m_TimeState(0),
      f_ProcessInput(processInput),
      f_Update(update),
      f_Render(render) {}

int Engine::Setup() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cout << "Error: SDL2 initialization failed! " << SDL_GetError()
                  << std::endl;
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        std::cout << "Error: SDL2_image initialization failed!" << std::endl;
        return 1;
    }
    if (TTF_Init() < 0) {
        std::cout << "Error: SDL2_ttf initialization failed!" << std::endl;
        return 1;
    }

    m_Window = SDL_CreateWindow(m_GameName.c_str(), SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (m_Window == NULL) {
        std::cout << "Error: SDL2 Window creation failed! " << SDL_GetError()
                  << std::endl;
        return 1;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);
    if (m_Renderer == NULL) {
        std::cout << "Error: SDL2 Renderer creation failed! " << SDL_GetError()
                  << std::endl;
        return 1;
    }

    SDL_RenderSetVSync(m_Renderer, 1);

    return 0;
}

void Engine::Cleanup() {
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);

    IMG_Quit();
    SDL_Quit();
}

#ifdef __EMSCRIPTEN__
void EMSCRIPT_EngineGameLoop(void* enginePtr) {
    Engine* engine = static_cast<Engine*>(enginePtr);

    engine->UpdateTime();

    engine->f_ProcessInput(engine->m_Event);
    engine->f_Update(engine->m_TimeState);
    engine->f_Render();
}
#endif

void Engine::Run() {
    // TODO: first few frames have very small delta except one (when window is
    //       created?), causes large jump in time
    m_TimeState = TimeState(SDL_GetPerformanceCounter());
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&EMSCRIPT_EngineGameLoop, this, 0, 1);
#else
    GameLoop();
#endif
}

void Engine::GameLoop() {
    while (!m_QueueQuit) {
        UpdateTime();

        f_ProcessInput(m_Event);
        f_Update(m_TimeState);
        f_Render();
    }
}

void Engine::UpdateTime() {
    m_TimeState.NextTimestep(SDL_GetPerformanceCounter(),
                             SDL_GetPerformanceFrequency());
}
}  // namespace Engine