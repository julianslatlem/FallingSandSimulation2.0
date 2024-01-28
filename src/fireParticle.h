#ifndef FIREPARTICLE_H
#define FIREPARTICLE_H

#include "globals.h"

// The fire particle class. Inherits from the particle class.
class FireParticle : public Particle {
public:
    FireParticle(const int x, const int y) {
        id = 4;
        position = {x, y};
        velocity = {};
        color = fireColor[rand() % 3];
    }

    // Checks if the given position is within the bounds of the world.
    static bool inBounds(const int x, const int y) {
        return x >= 0 && x < WIDTH / PARTICLE_SIZE && y >= 0 && y < HEIGHT / PARTICLE_SIZE;
    }

    void update() override {
        // Checks if the particle should be deleted. This is done randomly to prevent the fire from spreading too much.
        if (rand() % 10 == 0) {
            worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = nullptr;
        }
        else {
            // Adds a small random velocity to the particle
            this->velocity[0] += (static_cast<float>(rand() % 3 - 1)) * 0.5f; // Generates a pseudo-random value between -0.5 and 0.5.
            this->velocity[1] += (static_cast<float>(rand() % 3 - 1)) * 0.5f;

            // Calculates the new position based on the velocity.
            int newX = this->position[0] + static_cast<int>(this->velocity[0]);
            int newY = this->position[1] + static_cast<int>(this->velocity[1]);

            // Checks if the new position is within the bounds.
            if (inBounds(newX, newY)) {
                const Particle* otherParticle = worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX];
                if (otherParticle == nullptr) {
                    // Moves the particle to the new position.
                    worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = nullptr;
                    worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX] = this;
                    this->position[0] = newX;
                    this->position[1] = newY;
                } else if (otherParticle->id == 3) {
                    // Converts the other particle to fire.
                    delete otherParticle;
                    worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX] = new FireParticle(newX, newY);
                }
            } else {
                // Resets the velocity if the new position is not valid.
                this->velocity[0] = 0;
                this->velocity[1] = 0;
            }

            // Check if the next position will be a particle with an id of 3.
            newX = this->position[0] + 2 * static_cast<int>(this->velocity[0]); // Interpolates two iterations into the future.
            newY = this->position[1] + 2 * static_cast<int>(this->velocity[1]); // Interpolates two iterations into the future.

            if (inBounds(newX, newY)) {
                const Particle* otherParticle = worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX];
                if (otherParticle != nullptr && otherParticle->id == 3) {
                    // Converts the other particle to fire.
                    delete otherParticle;
                    worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX] = new FireParticle(newX, newY);
                }
            }
        }

        this->updatedThisFrame = true; // Sets the updatedThisFrame variable to true, so that the particle is not updated again this frame.
    }

    void render(SDL_Renderer* renderer, const int x, const int y) override {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

        const SDL_Rect rect = {x * PARTICLE_SIZE, y * PARTICLE_SIZE, PARTICLE_SIZE, PARTICLE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
};

#endif //FIREPARTICLE_H
