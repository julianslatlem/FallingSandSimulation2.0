#ifndef WATERPARTICLE_H
#define WATERPARTICLE_H

#include "globals.h"

class WaterParticle : public Particle {
public:
    WaterParticle(int x, int y) {
        id = 3;
        position = {x, y};
        velocity = {0, 0};
        color = waterColor[rand() % 3];
    }

    void update() override {
        int x = this->position[0];
        int y = this->position[1];

        // Apply a smaller constant to the y-component of the velocity
        this->velocity[1] += GRAVITY / 50.0f;

        // Calculate the new position based on the velocity
        int newX = x + this->velocity[0];
        int newY = y + this->velocity[1];

        // Determine the direction of movement
        int dx = (newX > x) ? 1 : (newX < x) ? -1 : 0;
        int dy = (newY > y) ? 1 : (newY < y) ? -1 : 0;

        // Move the particle one step at a time
        while (x != newX || y != newY) {
            if (x != newX) {
                x += dx;
            }
            if (y != newY) {
                y += dy;
            }

            // Check if the new position is within the bounds and empty
            if (x >= 0 && x < WIDTH / PARTICLE_SIZE && y < HEIGHT / PARTICLE_SIZE && worldParticleData[y * (WIDTH / PARTICLE_SIZE) + x] == nullptr) {
                // Move the particle to the new position
                worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = nullptr;
                worldParticleData[y * (WIDTH / PARTICLE_SIZE) + x] = this;
                this->position[0] = x;
                this->position[1] = y;
            } else {
                // If the new position is not valid, handle the collision and break out of the loop
                this->velocity[0] = 0;
                this->velocity[1] = 0;
                break;
            }
        }

        // Check for empty spaces to the sides of the particle
        std::array<int, 3> directions = {-1, 1};
        std::random_shuffle(directions.begin(), directions.end());

        for (int dx : directions) {
            int sideX = this->position[0] + dx;
            int sideY = this->position[1];
            if (sideX >= 0 && sideX < WIDTH / PARTICLE_SIZE && sideY < HEIGHT / PARTICLE_SIZE && worldParticleData[sideY * (WIDTH / PARTICLE_SIZE) + sideX] == nullptr) {
                // Move the particle to the empty space
                worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = nullptr;
                worldParticleData[sideY * (WIDTH / PARTICLE_SIZE) + sideX] = this;
                this->position[0] = sideX;
                this->position[1] = sideY;
                break;
            }
        }

        // Check for sand particles above the water particle
        int aboveX = this->position[0];
        int aboveY = this->position[1] - 1;
        if (aboveX >= 0 && aboveX < WIDTH / PARTICLE_SIZE && aboveY >= 0) {
            Particle* aboveParticle = worldParticleData[aboveY * (WIDTH / PARTICLE_SIZE) + aboveX];
            if (aboveParticle != nullptr && aboveParticle->id == 1) { // Assuming sand particle id is 1
                // Swap the positions of the water and sand particles
                worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = aboveParticle;
                worldParticleData[aboveY * (WIDTH / PARTICLE_SIZE) + aboveX] = this;
                aboveParticle->position[0] = this->position[0];
                aboveParticle->position[1] = this->position[1];
                this->position[0] = aboveX;
                this->position[1] = aboveY;
            }
        }

        updatedThisFrame = true;
    }

    void render(SDL_Renderer* renderer, int x, int y) override {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_Rect rect = {x * PARTICLE_SIZE, y * PARTICLE_SIZE, PARTICLE_SIZE, PARTICLE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
};

#endif //WATERPARTICLE_H
