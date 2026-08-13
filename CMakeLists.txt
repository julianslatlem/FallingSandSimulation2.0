cmake_minimum_required(VERSION 3.12)
project(fallingSandSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable(fallingSandSimulation
    src/main.cpp
    src/globals.h
    src/sandParticle.h
    src/stoneParticle.h
    src/gunpowderParticle.h
    src/fireParticle.h
)

target_include_directories(fallingSandSimulation PRIVATE
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(fallingSandSimulation PRIVATE
    ${SDL2_LIBRARIES}
)
