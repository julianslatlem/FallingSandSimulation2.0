#include <SDL.h>
#include "globals.h"
#include <random>

#include "sandParticle.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return EXIT_FAILURE; // Initializes the SDL library.

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Falling Sand Simulation C++", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) { SDL_Quit(); return EXIT_FAILURE; } // Checks if the window was created successfully.

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) { SDL_DestroyWindow(window); SDL_Quit(); return EXIT_FAILURE; } // Checks if the renerer was created successfully.

    worldParticleData = new Particle*[(WIDTH / PARTICLE_SIZE) * (HEIGHT / PARTICLE_SIZE)](); // Allocate memory for worldParticleData.

    // Use a random number generator.
    std::random_device rd;
    std::mt19937 g(rd());

    // Create a vector of indices
    std::vector<int> indices((WIDTH / PARTICLE_SIZE) * (HEIGHT / PARTICLE_SIZE));
    std::iota(indices.begin(), indices.end(), 0); // Fill it with consecutive numbers.

    SDL_Event e;
    bool running = true;
    while (running) { // Creates a game loop that runs until the user closes the window.
        uint32_t startTime = SDL_GetTicks(); // Get the start time.

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false; // Checks if the user closed the window.

            // Handle mouse wheel event.
            if (e.type == SDL_MOUSEWHEEL) {
                // Increase or decrease brushSize based on the amount scrolled.
                brushSize += e.wheel.y;
                // Ensure brushSize is within a certain range, for example, 1 to 100.
                brushSize = std::max(1, std::min(brushSize, 100));
            }

            mouseState = SDL_GetMouseState(&mouse[0], &mouse[1]);
        }

        if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && mouse[0] >= 0 && mouse[0] < WIDTH && mouse[1] >= 0 && mouse[1] < HEIGHT) {
            for (int i = -brushSize; i <= brushSize; i++) {
                for (int j = -brushSize; j <= brushSize; j++) {
                    // Check if the current position is within a circle of radius brushSize.
                    if (i * i + j * j <= brushSize * brushSize) {
                        int brushX = (mouse[0] / PARTICLE_SIZE) + i;
                        int brushY = (mouse[1] / PARTICLE_SIZE) + j;
                        if (brushX >= 0 && brushX < WIDTH / PARTICLE_SIZE && brushY >= 0 && brushY < HEIGHT / PARTICLE_SIZE) {
                            worldParticleData[brushY * (WIDTH / PARTICLE_SIZE) + brushX] = new SandParticle(brushX, brushY);
                        }
                    }
                }
            }
        }
        else if ((mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) && mouse[0] >= 0 && mouse[0] < WIDTH && mouse[1] >= 0 && mouse[1] < HEIGHT) {
            for (int i = -brushSize; i <= brushSize; i++) {
                for (int j = -brushSize; j <= brushSize; j++) {
                    // Check if the current position is within a circle of radius brushSize.
                    if (i * i + j * j <= brushSize * brushSize) {
                        int brushX = (mouse[0] / PARTICLE_SIZE) + i;
                        int brushY = (mouse[1] / PARTICLE_SIZE) + j;
                        if (brushX >= 0 && brushX < WIDTH / PARTICLE_SIZE && brushY >= 0 && brushY < HEIGHT / PARTICLE_SIZE) {
                            worldParticleData[brushY * (WIDTH / PARTICLE_SIZE) + brushX] = nullptr;
                        }
                    }
                }
            }
        }

        // Clears the screen.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // // Goes through every particle in the world and renders it.
        // for (int y = 0; y < (HEIGHT / PARTICLE_SIZE); y++) { // For every row of pixels.
        //     for (int x = 0; x < (WIDTH / PARTICLE_SIZE); x++) { // For every pixel in the row.
        //         Particle* particle = worldParticleData[y * (WIDTH / PARTICLE_SIZE) + x]; // Gets the particle at the current position.
        //         if (particle != nullptr && particle->id != 0) {
        //             if (!particle->updatedThisFrame) { particle->update(); particle->render(renderer, x, y); }
        //         }
        //     }
        // }
        //
        // for (int i = 0; i < (WIDTH / PARTICLE_SIZE) * (HEIGHT / PARTICLE_SIZE); i++) {
        //     Particle* particle = worldParticleData[i];
        //     if (particle != nullptr && particle->id != 0) particle->updatedThisFrame = false;
        // }

        // Shuffle the indices.
        std::shuffle(indices.begin(), indices.end(), g);

        // Go through every particle in the world in a random order and render it.
        for (int i : indices) {
            Particle* particle = worldParticleData[i];
            if (particle != nullptr && particle->id != 0) {
                if (!particle->updatedThisFrame) { particle->update(); particle->render(renderer, i % (WIDTH / PARTICLE_SIZE), i / (WIDTH / PARTICLE_SIZE)); }
            }
        }

        for (int i : indices) {
            Particle* particle = worldParticleData[i];
            if (particle != nullptr && particle->id != 0) particle->updatedThisFrame = false;
        }

        SDL_RenderPresent(renderer); // Displays the rendered pixel buffer to the screen.

        // Get the end time.
        Uint32 endTime = SDL_GetTicks();

        // Calculate the frame time.
        Uint32 frameTime = endTime - startTime;

        // If the frame time is less than the target frame time, delay the game loop.
        if (frameTime < TARGET_FRAME_TIME) {
            SDL_Delay(TARGET_FRAME_TIME - frameTime);
        }
    }

    delete[] worldParticleData; // Deallocate memory for worldParticleData.

    // Frees memory and quits SDL.
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return EXIT_SUCCESS; // Exits the program.
}
