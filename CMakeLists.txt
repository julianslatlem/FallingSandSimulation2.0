cmake_minimum_required(VERSION 3.27)
project(fallingSandSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "deps")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(fallingSandSimulation
        src/main.cpp
        src/globals.h
        src/sandParticle.h
        src/stoneParticle.h
        src/waterParticle.h
        src/gunpowderParticle.h
        src/fireParticle.h
)

target_link_libraries(fallingSandSimulation ${SDL2_LIBRARY} ${SDL2_Mixer_LIBRARY})
