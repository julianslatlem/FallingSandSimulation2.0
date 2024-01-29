#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector> // Includes the vector library for vectors.
#include <array> // Includes the array library for arrays.
#include <random> // Includes the random library for generating random numbers.
#include <algorithm> // Includes the algorithm library for shuffling arrays.

// Global constants. These are used throughout the program, and can be modified before building.
constexpr int TARGET_FPS = 60;
constexpr int TARGET_FRAME_TIME = 1000 / TARGET_FPS; // In milliseconds.

constexpr int WIDTH = 1920; // The width of the window.
constexpr int HEIGHT = 1080; // The height of the window.
constexpr float GRAVITY = 9.81f; // The gravity constant. Used for particles that are affected by gravity.
constexpr int PARTICLE_SIZE = 4; // The size of each particle.

// A boolean that determines if the simulation is paused or not.
inline int paused = false;

// Global variables. These are used throughout the program, and can be modified while the program is running.
inline int brushSize = 5;

// The mouse position and state. The mouse position is updated every frame, and the mouse state is updated when the mouse is clicked.
inline int mouse[2] = {0, 0};
inline uint32_t mouseState;

// The last mouse position. Used to determine the direction of the mouse movement.
inline int lastMouse[2] = {0, 0};

inline std::random_device rd; // A random device used to generate random numbers.

// A color type that stores the red, green, and blue components of a color.
typedef struct color_t {
    uint8_t r, g, b;
} color_t;

// Colors for each particle to choose from randomly. Used to get variation in the particle colors.
constexpr color_t sandColor[3] = {{246, 215, 176}, {236, 204, 162}, {225, 191, 146}};
constexpr color_t stoneColor[3] = {{150, 150, 152}, {140, 140, 142}, {130, 130, 132}};
constexpr color_t gunpowderColor[3] = {{30, 30, 30}, {25, 25, 25}, {20, 20, 20}};
constexpr color_t fireColor[3] = {{255, 0, 0}, {255, 90, 0}, {255, 154, 0}};


// A particle class that is used as a base class for all the different particle types.
class Particle {
public:
    uint8_t id{}; // The ID of the particle. Used to determine the type of the particle.
    float lifeTime{}; // The max life time of the particle. Currently unused.
    std::array<int, 2> position{}; // The x and y position of the particle.
    std::array<float, 2> velocity{}; // The x and y velocity of the particle.
    color_t color{}; // The color of the particle.
    bool updatedThisFrame{}; // Checks if the particle has been updated this iteration to prevent it from being updated multiple times.

    virtual void update() = 0; // The simulation function for the particle. To be implemented by the child classes.
    virtual void render(SDL_Renderer* renderer, int x, int y) = 0; // The rendering function for the particle. To be implemented by the child classes.

    virtual ~Particle() = default; // The destructor for the particle.
};

// The array of particles that make up the world. Defined as a pointer instead of an actual array to be more efficient.
inline Particle** worldParticleData;

enum class ParticleType {
    Sand,
    Stone,
    Gunpowder,
    Fire
};

// A list of all the particle types, as well as an index to cycle through it.
inline std::vector<ParticleType> particleTypes = {ParticleType::Sand, ParticleType::Stone, ParticleType::Gunpowder, ParticleType::Fire};
inline int currentParticleTypeIndex = 0;

#endif //GLOBALS_H
