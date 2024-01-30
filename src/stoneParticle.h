#ifndef STONEPARTICLE_H
#define STONEPARTICLE_H

#include "globals.h"

// The stone particle class. Inherits from the particle class.
class StoneParticle : public Particle {
public:
    StoneParticle(const int x, const int y) {
        id = 2;
        position = {x, y};
        velocity = {};
        color = stoneColor[rand() % 3];
    }

    void update() override {
        // Do nothing, as the stone particle simply stays put.
    }

    void render(SDL_Renderer* renderer, const int x, const int y) override {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

        const SDL_Rect rect = {x * PARTICLE_SIZE, y * PARTICLE_SIZE, PARTICLE_SIZE, PARTICLE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    void updateColor() override {
        // Do nothing, as the stone particle does not change color.
    }
};

#endif //STONEPARTICLE_H
