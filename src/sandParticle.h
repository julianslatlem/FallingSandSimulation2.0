#ifndef SANDPARTICLE_H
#define SANDPARTICLE_H

#include "globals.h"

// The sand particle class. Inherits from the particle class.
class SandParticle : public Particle {
public:
    SandParticle(const int x, const int y) {
        id = 1;
        position = {x, y};
        velocity = {};

        color_t newSandColor = sandColor[rand() % 3];

        newSandColor.r += static_cast<uint8_t>(sandColorMask);
        newSandColor.g += static_cast<uint8_t>(sandColorMask);
        newSandColor.b += static_cast<uint8_t>(sandColorMask);

        color = newSandColor;
    }

    std::default_random_engine eng{rd()}; // A random engine used to generate random numbers.

    void update() override {
        int x = this->position[0];
        int y = this->position[1];

        // Applies gravity to the y-component of the velocity.
        this->velocity[1] += GRAVITY / 50.0f; // Divides by 50 to make the gravity effect weaker.

        // Calculates the new position based on the velocity.
        const int newX = x + static_cast<int>(this->velocity[0]);
        const int newY = y + static_cast<int>(this->velocity[1]);

        // Determines the direction of movement.
        const int dx = (newX > x) ? 1 : (newX < x) ? -1 : 0;
        const int dy = (newY > y) ? 1 : (newY < y) ? -1 : 0;

        // Moves the particle one step at a time towards the new position.
        while (x != newX || y != newY) {
            if (x != newX) {
                x += dx;
            }
            if (y != newY) {
                y += dy;
            }

            // Checks if the new position is within the bounds and empty.
            if (x >= 0 && x < WIDTH / PARTICLE_SIZE && y < HEIGHT / PARTICLE_SIZE && worldParticleData[y * (WIDTH / PARTICLE_SIZE) + x] == nullptr) {
                // Moves the particle to the new position.
                worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = nullptr;
                worldParticleData[y * (WIDTH / PARTICLE_SIZE) + x] = this;
                this->position[0] = x;
                this->position[1] = y;
            } else {
                // Reset the velocity if the new position is not valid.
                this->velocity[0] = 0;
                this->velocity[1] = 0;
                break;
            }
        }

        // Check for empty spaces below and to the sides of the particle
        std::array<int, 3> directions = {-1, 0, 1};
        std::shuffle(directions.begin(), directions.end(), eng); // Shuffles the directions array.

        // Loops through the directions array and checks if there is an empty space below the particle.
        for (const int dx : directions) {
            const int belowX = this->position[0] + dx;
            const int belowY = this->position[1] + 1;
            if (belowX >= 0 && belowX < WIDTH / PARTICLE_SIZE && belowY < HEIGHT / PARTICLE_SIZE && worldParticleData[belowY * (WIDTH / PARTICLE_SIZE) + belowX] == nullptr) {
                // Moves the particle to the empty space.
                worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = nullptr;
                worldParticleData[belowY * (WIDTH / PARTICLE_SIZE) + belowX] = this;
                this->position[0] = belowX;
                this->position[1] = belowY;
                break;
            }
        }

        updatedThisFrame = true; // Sets the updatedThisFrame variable to true, so that the particle is not updated again this frame.
    }

    void render(SDL_Renderer* renderer, const int x, const int y) override {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

        const SDL_Rect rect = {x * PARTICLE_SIZE, y * PARTICLE_SIZE, PARTICLE_SIZE, PARTICLE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
};

#endif //SANDPARTICLE_H
