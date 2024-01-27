#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <array>

// Global constants. These are used throughout the program, and can be modified before building.
constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;
constexpr float GRAVITY = 9.81f;
constexpr int PARTICLE_SIZE = 4;

constexpr int TARGET_FPS = 60;
constexpr int TARGET_FRAME_TIME = 1000 / TARGET_FPS; // in milliseconds

// Global variables. These are used throughout the program, and can be modified while the program is running.
inline int brushSize = 5;

typedef struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_t;

// Particle colors.
constexpr color_t sandColor[3] = {{246, 215, 176}, {236, 204, 162}, {225, 191, 146}};
constexpr color_t stoneColor[3] = {{150, 150, 152}, {140, 140, 142}, {130, 130, 132}};
constexpr color_t gunpowderColor[3] = {{30, 30, 30}, {25, 25, 25}, {20, 20, 20}};
constexpr color_t fireColor[3] = {{255, 0, 0}, {255, 90, 0}, {255, 154, 0}};
//constexpr color_t waterColor[3] = {{0, 43, 96}, {1, 46, 102}, {2, 47, 102}};

class Particle {
public:
    uint8_t id;
    float lifeTime;
    std::array<int, 2> position;
    std::array<float, 2> velocity;
    color_t color;
    bool updatedThisFrame;

    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer, int x, int y) = 0;
};

inline Particle** worldParticleData;

enum class ParticleType {
    Sand,
    Stone,
    Gunpowder,
    Fire,
    //Water
};

// Create a list of particle types
inline std::vector<ParticleType> particleTypes = {ParticleType::Sand, ParticleType::Stone, ParticleType::Gunpowder, ParticleType::Fire};
inline int currentParticleTypeIndex = 0;

inline int mouse[2] = {0, 0};
inline uint32_t mouseState;

#endif //GLOBALS_H
