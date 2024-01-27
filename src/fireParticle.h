#ifndef FIREPARTICLE_H
#define FIREPARTICLE_H

#include "globals.h"

class FireParticle : public Particle {
public:
    FireParticle(int x, int y) {
        id = 4;
        position = {x, y};
        velocity = {0, 0};
        color = fireColor[rand() % 3];
    }

    bool inBounds(int x, int y) {
        return x >= 0 && x < WIDTH / PARTICLE_SIZE && y >= 0 && y < HEIGHT / PARTICLE_SIZE;
    }

    void update() override {
        bool die = rand() % 10 == 0;

        if (die) {
            worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = nullptr;
        }
        else {
            // Add a small random velocity to the particle
            this->velocity[0] += (rand() % 3 - 1) * 0.5f; // Random value between -0.5 and 0.5
            this->velocity[1] += (rand() % 3 - 1) * 0.5f; // Random value between -0.5 and 0.5

            // Calculate the new position based on the velocity
            int newX = this->position[0] + this->velocity[0];
            int newY = this->position[1] + this->velocity[1];

            // Check if the new position is within the bounds
            if (inBounds(newX, newY)) {
                Particle* otherParticle = worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX];
                if (otherParticle == nullptr) {
                    // Move the particle to the new position
                    worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = nullptr;
                    worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX] = this;
                    this->position[0] = newX;
                    this->position[1] = newY;
                } else if (otherParticle->id == 3) {
                    // Convert the other particle to fire
                    delete otherParticle;
                    worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX] = new FireParticle(newX, newY);
                }
            } else {
                // If the new position is not valid, reset the velocity
                this->velocity[0] = 0;
                this->velocity[1] = 0;
            }

            // Check if the next position will be a particle with id "3"
            newX = this->position[0] + 2 * this->velocity[0]; // Interpolate two iterations into the future
            newY = this->position[1] + 2 * this->velocity[1]; // Interpolate two iterations into the future

            if (inBounds(newX, newY)) {
                Particle* otherParticle = worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX];
                if (otherParticle != nullptr && otherParticle->id == 3) {
                    // Convert the other particle to fire
                    delete otherParticle;
                    worldParticleData[newY * (WIDTH / PARTICLE_SIZE) + newX] = new FireParticle(newX, newY);
                }
            }
        }
    }

    void render(SDL_Renderer* renderer, int x, int y) override {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_Rect rect = {x * PARTICLE_SIZE, y * PARTICLE_SIZE, PARTICLE_SIZE, PARTICLE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
};

#endif //FIREPARTICLE_H
