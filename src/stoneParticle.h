#ifndef STONEPARTICLE_H
#define STONEPARTICLE_H

#include "globals.h"

class StoneParticle : public Particle {
public:
    StoneParticle(int x, int y) {
        id = 2;
        position = {x, y};
        velocity = {0, 0};
        color = stoneColor[rand() % 3];
    }

    void update() override {
        // Do nothing.
    }

    void render(SDL_Renderer* renderer, int x, int y) override {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_Rect rect = {x * PARTICLE_SIZE, y * PARTICLE_SIZE, PARTICLE_SIZE, PARTICLE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
};

#endif //STONEPARTICLE_H
