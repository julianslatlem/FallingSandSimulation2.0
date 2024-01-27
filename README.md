# Falling Sand Simulation

## Description
The goal of this project was to recreate the famous game **_Powder Toy_**. <br>
This simulation is of course way less complex than the original game, but it still has some interesting features. <br>

The simulation is made in C++ using the SDL library for the graphics. <br>

## How to play
Just place the particles on the screen and watch them interact with each other. <br>
Place particle: [LEFT MOUSE BUTTON] <br>
Remove particle: [RIGHT MOUSE BUTTON] <br>
Change brush size: [SCROLL UP/DOWN] <br>
Change particle: [TAB] <br>

## How to build
To build the project, you'll need to have a cmake installed. <br>
Once you have that, you can run the following commands in the terminal: <br>
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
This will create an executable in the build folder. <br>
This is a cross-platform method, so it should work on both Linux and Windows, assuming you have a C++ compiler installed. <br>

To run it you'll need to have the SDL2 library installed. <br>
On Linux, this is easy, simply run the following command in the terminal: <br>
```bash
sudo pacman -S libsdl2-dev
```

On Windows, you will need to get a hold of the SDL2 dll from the SDL website. <br>
You can find it here: https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5 <br>
Extract it, and put SDL.dll in the same folder as the executable before execution. <br>

## How it works
Although it may seem complex, it is actually based on some simple rules. <br>
To sum it up, the game consists of a grid of particles, each particle having a color and a custom update function attached. <br>
The update function is called every frame, and it is responsible for the particle's behavior. <br>

The sand particle for instance, has a simple update function that checks first if there is an empty space below it, and if not, it checks down to the left, and if that also is taken... you get the idea. <br>

The fire is also quite simple, as the way it spreads is simply by checking the id of the surrounding particles, and if it is a flammable material, it will turn it into fire. <br>


## The advantages of C++
Well, first of all, it is fast. <br>
Compared to the Python version of this project, which is noticeably dissimilar; this simulation runs at a much higher framerate. <br>
Not only that, but it also has a much higher resolution, and it can handle a lot more particles on the screen at the same time. <br>

This, combined with the fact that C++ has a lot more features than Python, like pointers, makes it a lot easier to implement new features. <br>

I will say however that Python is at a huge disadvantage here, as it isn't really meant for this kind of stuff. <br>
Sure, it is a lot easier to write code in Python in _some_ instances, and it _can_ be easier to debug, but it's also just so slow that you can barely do anything with it apart from simple calculations and data-visualization. <br>