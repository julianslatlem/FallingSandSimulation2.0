#include <SDL.h>
#include "globals.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return EXIT_FAILURE;

    SDL_Window* window = SDL_CreateWindow("Falling Sand Simulation C++", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == nullptr) { SDL_Quit(); return EXIT_FAILURE; }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == nullptr) { SDL_DestroyWindow(window); SDL_Quit(); return EXIT_FAILURE; }

    SDL_Event e;

    bool running = true;
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }

            mouseState = SDL_GetMouseState(&mouse[0], &mouse[1]);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (&worldParticleData[x * y] != nullptr) {
                    SDL_Rect rect = {x, y, 1, 1};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }

    return EXIT_SUCCESS;
}