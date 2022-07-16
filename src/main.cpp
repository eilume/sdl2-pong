#include <iostream>

#include "engine.h"

Engine::Engine* engine;
SDL_Texture* ballTex = NULL;

void ProcessInput(SDL_Event& event) {
    while (SDL_PollEvent(&event) > 0) {
        if (event.type == SDL_QUIT) engine->QueueQuit();
    }
}

void Update(Engine::TimeState& timestate) {}

void Render() {
    SDL_SetRenderDrawColor(engine->GetRenderer(), 130, 47, 82, 255);
    SDL_RenderClear(engine->GetRenderer());
    SDL_RenderCopy(engine->GetRenderer(), ballTex, NULL, NULL);
    SDL_RenderPresent(engine->GetRenderer());
}

int main(int argc, char** args) {
    engine = new Engine::Engine("Pong", ProcessInput, Update, Render);

    int engineSetup = engine->Setup();
    if (engineSetup != 0) return engineSetup;

    SDL_Surface* tmpSurface = IMG_Load(ASSETS_PATH "ball.png");
    ballTex = SDL_CreateTextureFromSurface(engine->GetRenderer(), tmpSurface);
    if (ballTex == NULL) {
        std::cout << "Error creating texture";
        return 1;
    }
    SDL_FreeSurface(tmpSurface);

    engine->Run();

    SDL_DestroyTexture(ballTex);
    engine->Cleanup();

    return 0;
}