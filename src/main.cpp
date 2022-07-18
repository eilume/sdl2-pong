#include <cstring>
#include <random>
#include <sstream>
#include <string>

#include "collision.h"
#include "config.h"
#include "engine.h"
#include "entity.h"
#include "font.h"
#include "particle.h"
#include "texture.h"

#ifdef __EMSCRIPTEN__
#define ASSETS_PATH "./assets/"
#endif

const int BALL_INITIAL_SPEED = 150;
const int BALL_SPEED_MAX = 1000;
const float BALL_BOUNCE_SPEED_MULTIPLIER = 1.08f;

const int PADDLE_SPEED = 350;
const int PADDLE_PADDING = 64;

const int PARTICLE_SYSTEM_COUNT = 8;

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

std::shared_ptr<Font> font;
Entity* score = nullptr;

std::vector<ParticleSystem> particleSystems;

bool keyDown[SDL_NUM_SCANCODES];

void UpdateScoreText() {
    if (score != nullptr) delete score;

    std::stringstream ss;
    ss << "SCORE " << playerOneScore << " : " << playerTwoScore;

    std::shared_ptr<Texture> scoreTex = font->TextToTexture(
        engine->GetRenderer(), const_cast<char*>(ss.str().c_str()),
        {255, 255, 255});
    score =
        new Entity(Vec2((SCREEN_WIDTH - scoreTex->GetDimensions()->x) / 2, 16),
                   Vec2(), scoreTex);
}

void StartParticleSystem(Vec2& startPos) {
    for (int i = 0; i < PARTICLE_SYSTEM_COUNT; i++) {
        if (!particleSystems[i].IsActive()) {
            particleSystems[i].Start(startPos);
            return;
        }
    }
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

#ifndef __EMSCRIPTEN__
    if (keyDown[SDL_SCANCODE_ESCAPE]) engine->QueueQuit();
#endif

    paddleOne->vel.y = 0.0f;
    if (keyDown[SDL_SCANCODE_W]) paddleOne->vel.y -= PADDLE_SPEED;
    if (keyDown[SDL_SCANCODE_S]) paddleOne->vel.y += PADDLE_SPEED;

    paddleTwo->vel.y = 0.0f;
    if (keyDown[SDL_SCANCODE_UP]) paddleTwo->vel.y -= PADDLE_SPEED;
    if (keyDown[SDL_SCANCODE_DOWN]) paddleTwo->vel.y += PADDLE_SPEED;
}

void Update(Engine::TimeState& timeState) {
    paddleOne->Update(timeState);
    paddleTwo->Update(timeState);
    ball->Update(timeState);

    // Perform paddle collisions with screen
    Collision::Collide(paddleOne, screenRect, true);
    Collision::Collide(paddleTwo, screenRect, true);

    paddleOne->UpdateRect();
    paddleTwo->UpdateRect();

    // Perform ball bounces
    BVec2 ballPaddleOneBounce = Collision::Bounce(ball, paddleOne->rect);
    BVec2 ballPaddleTwoBounce = Collision::Bounce(ball, paddleTwo->rect);
    BVec2 ballScreenBounce = Collision::Bounce(ball, screenRect, true);

    // If ball bounces on either the left or right side of the screen
    if (ballScreenBounce.x &&
        (ball->pos.x < PADDLE_PADDING ||
         ball->pos.x + ball->rect.w > SCREEN_WIDTH - PADDLE_PADDING)) {
        // Determine which player should get the score increase
        if (ball->pos.x > SCREEN_WIDTH / 2) {
            playerOneScore++;
        } else {
            playerTwoScore++;
        }

        UpdateScoreText();

        // Reset the ball
        ball->pos = Vec2(ballCenterPos.x, ballStartHeightDist(gen));
        ball->vel = GenerateBallStartVelocity();
    } else {
        // TODO: add velocity variation (not always perfect bouncing)
        if (ballPaddleOneBounce.Or() || ballPaddleTwoBounce.Or()) {
            Vec2 startPos = ball->pos + (*ball->tex->GetDimensions() / 2.0f);
            StartParticleSystem(startPos);

            if (ball->vel.Magnitude() < BALL_SPEED_MAX)
                ball->vel *= BALL_BOUNCE_SPEED_MULTIPLIER;
        }
    }

    ball->UpdateRect();

    for (int i = 0; i < PARTICLE_SYSTEM_COUNT; i++) {
        particleSystems[i].Update(timeState);
    }
}

void Render() {
    SDL_SetRenderDrawColor(engine->GetRenderer(), 130, 47, 82, 255);
    SDL_RenderClear(engine->GetRenderer());

    for (int i = 0; i < PARTICLE_SYSTEM_COUNT; i++) {
        particleSystems[i].Render(engine->GetRenderer());
    }

    paddleOne->Render(engine->GetRenderer());
    paddleTwo->Render(engine->GetRenderer());
    ball->Render(engine->GetRenderer());

    score->Render(engine->GetRenderer());

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

    font = Font::Load(ASSETS_PATH "FFFFORWA.ttf", 16);

    // Create ball
    std::shared_ptr<Texture> ballTex =
        Texture::Load(engine->GetRenderer(), ASSETS_PATH "ball.png");
    ballCenterPos = Vec2((SCREEN_WIDTH - ballTex->GetDimensions()->x) / 2,
                         (SCREEN_HEIGHT - ballTex->GetDimensions()->y) / 2);
    ballHeightRange = SCREEN_HEIGHT - ballTex->GetDimensions()->y;
    ball =
        new Entity(Vec2(ballCenterPos), GenerateBallStartVelocity(), ballTex);

    // Create paddles
    std::shared_ptr<Texture> paddleTex =
        Texture::Load(engine->GetRenderer(), ASSETS_PATH "paddle.png");
    paddleOne =
        new Entity(Vec2(PADDLE_PADDING,
                        (SCREEN_HEIGHT - paddleTex->GetDimensions()->y) / 2),
                   Vec2(), paddleTex);
    paddleTwo = new Entity(
        Vec2(SCREEN_WIDTH - PADDLE_PADDING - paddleTex->GetDimensions()->x,
             (SCREEN_HEIGHT - paddleTex->GetDimensions()->y) / 2),
        Vec2(), paddleTex);

    std::shared_ptr<Texture> particleTex =
        Texture::Load(engine->GetRenderer(), ASSETS_PATH "particle.png");

    particleSystems.reserve(PARTICLE_SYSTEM_COUNT);
    for (int i = 0; i < PARTICLE_SYSTEM_COUNT; i++) {
        particleSystems.emplace_back(32, particleTex, &gen);
    }

    ballStartHeightDist =
        std::uniform_int_distribution<int>(0, ballHeightRange);

    UpdateScoreText();

    engine->Run();

    delete paddleOne;
    delete paddleTwo;
    delete ball;

    engine->Cleanup();

    return 0;
}