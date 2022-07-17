#include <iostream>
#include <random>

#include "collision.h"
#include "config.h"
#include "engine.h"
#include "entity.h"

#ifdef __EMSCRIPTEN__
#define ASSETS_PATH "./assets/"
#endif

const float BALL_BOUNCE_SPEED_MULTIPLIER = 1.08f;
const int BALL_INITIAL_SPEED = 150;
const int BALL_SPEED_MAX = 1500;

const int PADDLE_SPEED = 300;
const int PADDLE_PADDING = 64;

Engine::Engine* engine;
SDL_Rect screenRect;

Entity* paddleOne;
Entity* paddleTwo;

Entity* ball;
Vec2 ballCenterPos;
int ballHeightRange;
std::uniform_int_distribution<int> ballStartVelDist;
std::uniform_int_distribution<int> ballStartHeightDist;

std::mt19937 gen;

int playerOneScore;
int playerTwoScore;

bool keyDown[SDL_NUM_SCANCODES];

void PrintScore() {
    std::cout << "Score: " << playerOneScore << " : " << playerTwoScore
              << std::endl;
}

Vec2 GenerateBallStartVelocity() {
    int x = ballStartVelDist(gen) * 2 - 1;
    int y = ballStartVelDist(gen) * 2 - 1;
    return Vec2(x * BALL_INITIAL_SPEED, y * BALL_INITIAL_SPEED);
}

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

    paddleOne->rect.x = paddleOne->pos.x;
    paddleOne->rect.y = paddleOne->pos.y;
    paddleTwo->rect.x = paddleTwo->pos.x;
    paddleTwo->rect.y = paddleTwo->pos.y;

    BVec2 ballPaddleOneBounce = Collision::Bounce(ball, paddleOne->rect);
    BVec2 ballPaddleTwoBounce = Collision::Bounce(ball, paddleTwo->rect);
    BVec2 ballScreenBounce = Collision::Bounce(ball, screenRect, true);

    if (ballScreenBounce.x && (ball->pos.x < PADDLE_PADDING || ball->pos.x + ball->rect.w > SCREEN_WIDTH - PADDLE_PADDING)) {
        if (ball->pos.x > SCREEN_WIDTH / 2) {
            playerOneScore++;
        } else {
            playerTwoScore++;
        }

        PrintScore();

        ball->pos = Vec2(ballCenterPos.x, ballStartHeightDist(gen));
        ball->vel = GenerateBallStartVelocity();
    } else {
        // TODO: add velocity variation (not always perfect bouncing)
        if ((ballPaddleOneBounce.Or() || ballPaddleTwoBounce.Or()) &&
            ball->vel.Magnitude() < BALL_SPEED_MAX)
            ball->vel = ball->vel * BALL_BOUNCE_SPEED_MULTIPLIER;
    }

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

    playerOneScore = 0;
    playerTwoScore = 0;

    screenRect.x = 0;
    screenRect.y = 0;
    screenRect.w = SCREEN_WIDTH;
    screenRect.h = SCREEN_HEIGHT;

    std::random_device rd;
    gen = std::mt19937(rd());
    ballStartVelDist = std::uniform_int_distribution<int>(0, 1);

    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        keyDown[i] = false;
    }

    // Create ball
    SDL_Surface* tmpSurface = IMG_Load(ASSETS_PATH "ball.png");
    ballCenterPos = Vec2((SCREEN_WIDTH - tmpSurface->w) / 2,
                         (SCREEN_HEIGHT - tmpSurface->h) / 2);
    ballHeightRange = SCREEN_HEIGHT - tmpSurface->h;
    ball = new Entity(
        Vec2(ballCenterPos), GenerateBallStartVelocity(),
        SDL_CreateTextureFromSurface(engine->GetRenderer(), tmpSurface),
        Vec2(tmpSurface->w, tmpSurface->h));
    if (ball->tex == NULL) {
        std::cout << "Error creating ball texture";
        return 1;
    }
    SDL_FreeSurface(tmpSurface);

    // Create paddles
    tmpSurface = IMG_Load(ASSETS_PATH "paddle.png");
    SDL_Texture* paddleTex =
        SDL_CreateTextureFromSurface(engine->GetRenderer(), tmpSurface);
    if (paddleTex == NULL) {
        std::cout << "Error creating paddle texture";
        return 1;
    }
    paddleOne =
        new Entity(Vec2(PADDLE_PADDING, (SCREEN_HEIGHT - tmpSurface->h) / 2),
                   Vec2(), paddleTex, Vec2(tmpSurface->w, tmpSurface->h));
    paddleTwo =
        new Entity(Vec2(SCREEN_WIDTH - PADDLE_PADDING - tmpSurface->w,
                        (SCREEN_HEIGHT - tmpSurface->h) / 2),
                   Vec2(), paddleTex, Vec2(tmpSurface->w, tmpSurface->h));
    SDL_FreeSurface(tmpSurface);

    ballStartHeightDist =
        std::uniform_int_distribution<int>(0, ballHeightRange);

    PrintScore();

    engine->Run();

    SDL_DestroyTexture(ball->tex);
    engine->Cleanup();

    return 0;
}