#include <SDL.h>

int main(int argc, char** args) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window =
        SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event input;

    SDL_RenderSetVSync(renderer, 1);

    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&input) > 0) {
            if (input.type == SDL_QUIT ||
                (input.type == SDL_KEYDOWN && input.key.keysym.sym &&
                 SDL_KeyCode::SDLK_ESCAPE))
                quit = true;
        }

        SDL_SetRenderDrawColor(renderer, 255, 89, 77, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}