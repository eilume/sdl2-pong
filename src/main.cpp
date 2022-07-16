#include <iostream>

#include "collision.h"
#include "config.h"
#include "engine.h"
#include "entity.h"

const float BALL_BOUNCE_SPEED_MULTIPLIER = 1.1f;
const int BALL_INITIAL_SPEED = 100;
const int BALL_SPEED_MAX = 1500;

const int PADDLE_SPEED = 300;
const int PADDLE_PADDING = 64;

Engine::Engine* engine;
Entity* paddleOne;
Entity* paddleTwo;
Entity* ball;
SDL_Rect screenRect;

bool keyDown[SDL_NUM_SCANCODES];

void ProcessInput(SDL_Event& event) {
    while (SDL_PollEvent(&event) > 0) {
        switch (event.type) {
            case SDL_QUIT:
                engine->QueueQuit();
                break;
            case SDL_KEYDOWN:
                keyDown[event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                keyDown[event.key.keysym.scancode] = false;
                break;
            default:
                break;
        }
    }

    if (keyDown[SDL_SCANCODE_ESCAPE]) engine->QueueQuit();

    paddleOne->vel.y = 0.0f;
    if (keyDown[SDL_SCANCODE_W]) paddleOne->vel.y -= PADDLE_SPEED;
    if (keyDown[SDL_SCANCODE_S]) paddleOne->vel.y += PADDLE_SPEED;

    paddleTwo->vel.y = 0.0f;
    if (keyDown[SDL_SCANCODE_UP]) paddleTwo->vel.y -= PADDLE_SPEED;
    if (keyDown[SDL_SCANCODE_DOWN]) paddleTwo->vel.y += PADDLE_SPEED;
}

void Update(Engine::TimeState& timestate) {
    // Update positions based on velocity
    paddleOne->pos =
        paddleOne->pos + (paddleOne->vel * timestate.GetDeltaTime());
    paddleTwo->pos =
        paddleTwo->pos + (paddleTwo->vel * timestate.GetDeltaTime());
    ball->pos = ball->pos + (ball->vel * timestate.GetDeltaTime());

    // Physics
    Collision::Collide(paddleOne, screenRect, true);
    Collision::Collide(paddleTwo, screenRect, true);

    BVec2 screenBounce = Collision::Bounce(ball, screenRect, true);
    if (screenBounce.Or() && ball->vel.Magnitude() < BALL_SPEED_MAX)
        ball->vel = ball->vel * BALL_BOUNCE_SPEED_MULTIPLIER;

    // Update rects with position
    paddleOne->rect.x = paddleOne->pos.x;
    paddleOne->rect.y = paddleOne->pos.y;
    paddleTwo->rect.x = paddleTwo->pos.x;
    paddleTwo->rect.y = paddleTwo->pos.y;
    ball->rect.x = ball->pos.x;
    ball->rect.y = ball->pos.y;
}

void Render() {
    SDL_SetRenderDrawColor(engine->GetRenderer(), 130, 47, 82, 255);
    SDL_RenderClear(engine->GetRenderer());
    SDL_RenderCopy(engine->GetRenderer(), paddleOne->tex, NULL,
                   &paddleOne->rect);
    SDL_RenderCopy(engine->GetRenderer(), paddleTwo->tex, NULL,
                   &paddleTwo->rect);
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

    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        keyDown[i] = false;
    }

    {
        SDL_Surface* tmpSurface = IMG_Load(ASSETS_PATH "ball.png");
        ball = new Entity(
            Vec2((SCREEN_WIDTH - tmpSurface->w) / 2,
                 (SCREEN_HEIGHT - tmpSurface->h) / 2),
            Vec2(BALL_INITIAL_SPEED, BALL_INITIAL_SPEED),
            SDL_CreateTextureFromSurface(engine->GetRenderer(), tmpSurface),
            Vec2(tmpSurface->w, tmpSurface->h));
        if (ball->tex == NULL) {
            std::cout << "Error creating ball texture";
            return 1;
        }
        SDL_FreeSurface(tmpSurface);
    }
    {
        SDL_Surface* tmpSurface = IMG_Load(ASSETS_PATH "paddle.png");
        SDL_Texture* paddleTex =
            SDL_CreateTextureFromSurface(engine->GetRenderer(), tmpSurface);
        if (paddleTex == NULL) {
            std::cout << "Error creating paddle texture";
            return 1;
        }
        paddleOne = new Entity(
            Vec2(PADDLE_PADDING, (SCREEN_HEIGHT - tmpSurface->h) / 2), Vec2(),
            paddleTex, Vec2(tmpSurface->w, tmpSurface->h));
        paddleTwo =
            new Entity(Vec2(SCREEN_WIDTH - PADDLE_PADDING - tmpSurface->w,
                            (SCREEN_HEIGHT - tmpSurface->h) / 2),
                       Vec2(), paddleTex, Vec2(tmpSurface->w, tmpSurface->h));
        SDL_FreeSurface(tmpSurface);
    }

    engine->Run();

    SDL_DestroyTexture(ball->tex);
    engine->Cleanup();

    return 0;
}