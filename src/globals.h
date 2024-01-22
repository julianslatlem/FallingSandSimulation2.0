#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>

#define WIDTH 1280
#define HEIGHT 720

float gravity = 10.0f;

const int particleSize = 10;
const int brushSize = 5;

struct particle_t {
    uint8_t id;
    float lifeTime;
    std::vector<std::vector<float>> velocity;
    uint32_t color;
    bool updatedThisFrame;
};

particle_t* worldParticleData = {0};

int mouse[2] = {0, 0};
Uint32 mouseState;

#endif //GLOBALS_H
