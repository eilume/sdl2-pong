#include "engine.h"

Engine::Engine* engine;

void ProcessInput(SDL_Event& event) {
    while (SDL_PollEvent(&event) > 0) {
        if (event.type == SDL_QUIT) engine->QueueQuit();
    }
}

void Update(Engine::TimeState& timestate) {}

void Render() {
    SDL_SetRenderDrawColor(engine->GetRenderer(), 255, 89, 77, 255);
    SDL_RenderClear(engine->GetRenderer());
    SDL_RenderPresent(engine->GetRenderer());
}

int main(int argc, char** args) {
    engine = new Engine::Engine("Pong", ProcessInput, Update, Render);

    engine->Run();

    return 0;
}