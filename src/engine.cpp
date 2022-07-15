#include "engine.h"

namespace Engine {
Engine::Engine(const std::string& gameName, CustomProcessInput* processInput,
               CustomUpdate* update, CustomRender* render)
    : m_GameName(gameName),
      m_TimeState(0),
      f_ProcessInput(processInput),
      f_Update(update),
      f_Render(render) {}

void Engine::Run() {
    Setup();
    GameLoop();
    Cleanup();
}

void Engine::Setup() {
    SDL_Init(SDL_INIT_EVERYTHING);

    m_Window =
        SDL_CreateWindow(m_GameName.c_str(), SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

    SDL_RenderSetVSync(m_Renderer, 1);

    m_TimeState = TimeState(SDL_GetPerformanceCounter());
}

void Engine::Cleanup() {
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
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