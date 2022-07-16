#include <iostream>

#include "config.h"
#include "engine.h"
#include "entity.h"
#include "collision.h"

Engine::Engine* engine;
Entity* ball;
SDL_Rect screenRect;

void ProcessInput(SDL_Event& event) {
    while (SDL_PollEvent(&event) > 0) {
        if (event.type == SDL_QUIT) engine->QueueQuit();
    }
}

void Update(Engine::TimeState& timestate) {
    ball->pos = ball->pos + (ball->vel * timestate.GetDeltaTime());

    BVec2 bounce = Collision::Bounce(ball, screenRect, true);
    if (bounce.x) ball->vel.x *= -1;
    if (bounce.y) ball->vel.y *= -1;
    if (bounce.Or() && ball->vel.Magnitude() < 1500)
        ball->vel = ball->vel * 1.1f;

    ball->rect.x = ball->pos.x;
    ball->rect.y = ball->pos.y;
}

void Render() {
    SDL_SetRenderDrawColor(engine->GetRenderer(), 130, 47, 82, 255);
    SDL_RenderClear(engine->GetRenderer());
    SDL_RenderCopy(engine->GetRenderer(), ball->tex, NULL, &ball->rect);
    SDL_RenderPresent(engine->GetRenderer());
}

int main(int argc, char** args) {
    engine = new Engine::Engine("Pong", ProcessInput, Update, Render);

    int engineSetup = engine->Setup();
    if (engineSetup != 0) return engineSetup;

    screenRect.x = 0;
    screenRect.y = 0;
    screenRect.w = SCREEN_WIDTH;
    screenRect.h = SCREEN_HEIGHT;

    SDL_Surface* tmpSurface = IMG_Load(ASSETS_PATH "ball.png");
    ball = new Entity(
        Vec2(64, 64), Vec2(100, 100),
        SDL_CreateTextureFromSurface(engine->GetRenderer(), tmpSurface),
        Vec2(tmpSurface->w, tmpSurface->h));
    if (ball->tex == NULL) {
        std::cout << "Error creating texture";
        return 1;
    }

    SDL_FreeSurface(tmpSurface);

    engine->Run();

    SDL_DestroyTexture(ball->tex);
    engine->Cleanup();

    return 0;
}