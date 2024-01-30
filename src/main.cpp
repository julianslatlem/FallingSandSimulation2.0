#include <SDL.h> // Includes the SDL library for creating windows and rendering graphics.
#include "globals.h" // Includes the globals.h header file.

// Includes all particle types.
#include "sandParticle.h"
#include "stoneParticle.h"
#include "gunpowderParticle.h"
#include "fireParticle.h"

// Defines the setPixel and drawCircle function.
void setPixel(SDL_Renderer* renderer, int x, int y, SDL_Color color);
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius);

// Defines the interpolate function. Used for drawing the particles in an interpolated way.
void interpolate(int x1, int y1, int x2, int y2);

// The main function. Where the program starts.
int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return EXIT_FAILURE; // Initializes the SDL library.

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Falling Sand Simulation C++", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) { SDL_Quit(); return EXIT_FAILURE; } // Checks if the window was created successfully, and exits if not.

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) { SDL_DestroyWindow(window); SDL_Quit(); return EXIT_FAILURE; } // Checks if the renderer was created successfully, and exits if not.

    // Allocates memory for worldParticleData. Basically just sets the size of the array.
    worldParticleData = new Particle*[(WIDTH / PARTICLE_SIZE) * (HEIGHT / PARTICLE_SIZE)]();

    // Generates a random seed to be used when generating random numbers.
    std::mt19937 g(rd());

    // Creates a vector of indices to be shuffled later.
    std::vector<int> indices((WIDTH / PARTICLE_SIZE) * (HEIGHT / PARTICLE_SIZE));
    std::iota(indices.begin(), indices.end(), 0); // Fills indices with consecutive numbers.

    SDL_Event e;
    bool running = true;

    bool spacePressed = false; // A boolean that determines if the space key is pressed. Used to prevent the simulation from pausing and unpausing multiple times.

    // Creates a loop that runs until running is false.
    while (running) {
        const uint32_t startTime = SDL_GetTicks(); // Get the start time.

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false; // Sets running to false if the user closes the window.

            // Handles mouse wheel events.
            if (e.type == SDL_MOUSEWHEEL) {
                // Increases or decreases brushSize based on the amount scrolled.
                brushSize += e.wheel.y;
                // Ensures brushSize is within the range [0, 100].
                brushSize = std::max(0, std::min(brushSize, 100));
            }

            // Gets the mouse position and state.
            mouseState = SDL_GetMouseState(&mouse[0], &mouse[1]);

            // Checks if the user has pressed a key.
            if (e.type == SDL_KEYDOWN) {
                // Checks if the key is tab.
                if (e.key.keysym.sym == SDLK_TAB) {
                    // Cycles through the different particles.
                    currentParticleTypeIndex = (currentParticleTypeIndex + 1) % static_cast<int>(particleTypes.size());
                }
                else if (e.key.keysym.sym == SDLK_SPACE && !spacePressed) {
                    // Pauses or unpauses the simulation.
                    paused = !paused;
                    spacePressed = true;
                }
            }

            if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    spacePressed = false;
                }
            }

            // Checks if the user has pressed any mouse button this frame.
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                lastMouse[0] = mouse[0]; // Updates the last mouse position.
                lastMouse[1] = mouse[1];
            }
        }

        // Checks if the user is pressing any mouse buttons.
        if (mouseState) {
            interpolate(lastMouse[0], lastMouse[1], mouse[0], mouse[1]);
            lastMouse[0] = mouse[0];
            lastMouse[1] = mouse[1];
        }

        // Clears the screen.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Shuffles the indices.
        std::shuffle(indices.begin(), indices.end(), g);

        // Goes through every particle in the world in a random order and renders it.
        for (const int i : indices) {
            Particle* particle = worldParticleData[i];
            if (particle != nullptr && particle->id != 0) {
                if (!particle->updatedThisFrame) {
                    if (!paused) { particle->update(); }
                    particle->render(renderer, i % (WIDTH / PARTICLE_SIZE), i / (WIDTH / PARTICLE_SIZE));
                }
            }
        }

        // Makes the color of the sand particles change slightly over time.
        sandColorMask += 0.1f * static_cast<float>(sandColorSwitch);
        if (sandColorMask >= 5.0f) {
            sandColorSwitch = -1;
        } else if (sandColorMask <= -15.0f) {
            sandColorSwitch = 1;
        }

        // Goes through every particle in the world in a random order and updates it.
        for (const int i : indices) {
            Particle* particle = worldParticleData[i];
            if (particle != nullptr && particle->id != 0) particle->updatedThisFrame = false;
        }

        // Draws the circle that shows the brush size at the current mouse position.
        drawCircle(renderer, mouse[0], mouse[1], brushSize * PARTICLE_SIZE);

        // Displays the rendered pixel buffer to the screen.
        SDL_RenderPresent(renderer);

        // Gets the end time.
        const Uint32 endTime = SDL_GetTicks();

        // Calculates the frame time.
        const Uint32 frameTime = endTime - startTime;

        // Delays the program if the frame time is less than the target frame time.
        if (frameTime < TARGET_FRAME_TIME) {
            SDL_Delay(TARGET_FRAME_TIME - frameTime);
        }
    }

    // Deallocates memory for worldParticleData.
    delete[] worldParticleData;

    // Frees memory and quits SDL.
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    // Exits the program.
    return EXIT_SUCCESS;
}

// Sets the color of a pixel at the specified position.
void setPixel(SDL_Renderer* renderer, const int x, const int y, const SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, x, y);
}

// Draws the outline of a circle at the specified position, with the specified radius.
void drawCircle(SDL_Renderer* renderer, const int centerX, const int centerY, const int radius) {
    int x = radius;
    int y = 0;
    int err = 0;

    constexpr SDL_Color color = {255, 255, 255, 255};

    while (x >= y) {
        // Draw the eight octants of the circle.
        setPixel(renderer, centerX + x, centerY + y, color);
        setPixel(renderer, centerX + y, centerY + x, color);
        setPixel(renderer, centerX - y, centerY + x, color);
        setPixel(renderer, centerX - x, centerY + y, color);
        setPixel(renderer, centerX - x, centerY - y, color);
        setPixel(renderer, centerX - y, centerY - x, color);
        setPixel(renderer, centerX + y, centerY - x, color);
        setPixel(renderer, centerX + x, centerY - y, color);

        if (err <= 0) {
            y++;
            err += 2*y + 1;
        }

        if (err > 0) {
            x--;
            err -= 2*x + 1;
        }
    }
}

// An interpolation function that relies on Bresenham's line algorithm.
void interpolate(int x1, int y1, const int x2, const int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    const int sx = (x1 < x2) ? 1 : -1;
    const int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        for (int i = -brushSize; i <= brushSize; i++) {
            for (int j = -brushSize; j <= brushSize; j++) {
                if (i * i + j * j <= brushSize * brushSize) {
                    const int brushX = (x1 / PARTICLE_SIZE) + i;
                    const int brushY = (y1 / PARTICLE_SIZE) + j;

                    // Checks if the brush is within the bounds of the screen.
                    if (brushX >= 0 && brushX < WIDTH / PARTICLE_SIZE && brushY >= 0 && brushY < HEIGHT / PARTICLE_SIZE) {
                        const int index = brushY * (WIDTH / PARTICLE_SIZE) + brushX; // Calculates the index of the particle at the current position.
                        if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && worldParticleData[index] == nullptr) { // If the user is left clicking.
                            switch (particleTypes[currentParticleTypeIndex]) { // Creates a new particle at the current position based on the current particle type.
                                case ParticleType::Sand:
                                    worldParticleData[index] = new SandParticle(brushX, brushY);
                                break;
                                case ParticleType::Stone:
                                    worldParticleData[index] = new StoneParticle(brushX, brushY);
                                break;
                                case ParticleType::Gunpowder:
                                    worldParticleData[index] = new GunpowderParticle(brushX, brushY);
                                break;
                                case ParticleType::Fire:
                                    worldParticleData[index] = new FireParticle(brushX, brushY);
                                break;
                            }
                        }
                        else if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) { // If the user is right clicking.
                            worldParticleData[index] = nullptr; // Deletes the particle at the current position.
                        }
                    }
                }
            }
        }

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}
