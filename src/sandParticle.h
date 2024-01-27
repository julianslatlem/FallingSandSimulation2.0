#ifndef SANDPARTICLE_H
#define SANDPARTICLE_H

#include "globals.h"

class SandParticle : public Particle {
public:
    SandParticle(int x, int y) {
        id = 1;
        position = {x, y};
        velocity = {0, 0};
        color = sandColor[rand() % 3];
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

        // Check for empty spaces below and to the sides of the particle
        std::array<int, 3> directions = {-1, 0, 1};
        std::random_shuffle(directions.begin(), directions.end());

        for (int dx : directions) {
            int belowX = this->position[0] + dx;
            int belowY = this->position[1] + 1;
            if (belowX >= 0 && belowX < WIDTH / PARTICLE_SIZE && belowY < HEIGHT / PARTICLE_SIZE && worldParticleData[belowY * (WIDTH / PARTICLE_SIZE) + belowX] == nullptr) {
                // Move the particle to the empty space
                worldParticleData[this->position[1] * (WIDTH / PARTICLE_SIZE) + this->position[0]] = nullptr;
                worldParticleData[belowY * (WIDTH / PARTICLE_SIZE) + belowX] = this;
                this->position[0] = belowX;
                this->position[1] = belowY;
                break;
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

#endif //SANDPARTICLE_H
