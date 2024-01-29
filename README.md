# Falling Sand Simulation (C++ and Python)

## Description
The goal of this project was to recreate the famous game **_Powder Toy_**. <br>
This simulation is of course way less complex than the original game, but it still has some interesting features. <br>

The first simulation is made in C++ using the SDL library for the graphics. <br>
The second simulation is made in Python using the Pygame library for the graphics. <br>

## How to play
Just place the particles on the screen and watch them interact with each other. <br>
Place particle: [LEFT MOUSE BUTTON] <br>
Remove particle: [RIGHT MOUSE BUTTON] <br>
Change brush size: [SCROLL UP/DOWN] <br>
Change particle: [TAB] <br>
Pause/unpause simulation: [SPACE]

## How to build (C++)
To build the project, you'll need to have a cmake installed. <br>
Once you have that, you can run the following commands in the terminal: <br>
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
This will create an executable in the build folder. <br>
This is a cross-platform method, so it should work on both Linux and Windows, assuming you have a C++ compiler installed,
although on Windows, it seems to not work correctly unless you are using CLion for some reason.

## Variables
If you were to actually build the project yourself, there are a few variables that you can change in the code to alter the behavior of the simulation. <br>

Why would you want to do this? <br>
Well, because you can, of course. <br>

You can find the variables in the "src/globals.h" file. <br>
Here, you have variables such as:
- **WIDTH, HEIGHT** - The size of the window, in pixels.
- **PARTICLE_SIZE** - The size of each particle, in pixels.
- **TARGET_FPS** - The target framerate of the simulation.
- **GRAVITY** - The acceleration constant acting on the particles.

## How it works
Although it may seem complex, it is actually based on some simple rules. <br>
To sum it up, the game consists of a grid of particles, each particle having a color and a custom update function attached. <br>
The update function is called every frame, and it is responsible for the particle's behavior. <br>

The sand particle for instance, has a simple update function that checks first if there is an empty space below it, and if not, it checks down to the left, and if that also is taken, it checks down to the right... you get the idea. <br>
![Sand logic](.resources/sand-rules.png) <br>
It also has velocity, of course, which basically just changes how many iterations into the future it does at a time, also known as _interpolating_; <br>
so if the particle's velocity is 2, it will simulate it twice in a single update, making it fall twice as fast. <br>
At least that's the gist of it.

The fire is also quite simple, as the way it spreads is simply by checking the id of the surrounding particles, and if it is a flammable material, it will turn it into fire.

## The advantages of C++
Well, first of all, it is fast. <br>
Compared to the Python version of this project, which is noticeably dissimilar; this simulation runs at a much higher framerate. <br>
Not only that, but it also has a much higher resolution, and it can handle a lot more particles on the screen at the same time. <br>

This, combined with the fact that C++ has a lot more features than Python, like pointers, makes it a lot easier to implement new features. <br>

I will say however that Python is at a huge disadvantage here, as it isn't really meant for this kind of stuff. <br>
Sure, it is a lot easier to write code in Python in _some_ instances, and it _can_ be easier to debug, but it's also just so slow that you can barely do anything with it apart from simple calculations and data-visualization. <br>

## Conclusion
All in all, I think this project was a success. <br>
It was a lot of fun to make, and I learned a lot about these kinds of simulations in the process. <br>
I went through a lot of trial and error, and also a lot of iterations, but I think this is a good thing, as, just look at how much I have improved since the first version of this project. <br>
I also think that this project is a good example of how much of a difference the language you use can make, and how important it is to choose the right language for the job. <br>

## Screenshots
![Screenshot 1](screenshots/sand.png)
![Screenshot 1](screenshots/gunpowder.png)
![Screenshot 1](screenshots/fire.png)

## Sources
During the making of this project, I used a lot of different sources to learn how to create a proper simulation. <br>
Among these sources are the following: <br>
- [The Powder Toy](https://powdertoy.co.uk/) - The original game that inspired this project.
- [Some random YouTube video](https://www.youtube.com/watch?v=VLZjd_Y1gJ8) - This video was the first thing I watched when I started this project, and it helped me understand the basics of how to create a simulation like this.
- [GitHub Copilot](https://copilot.github.com/) - I used this quite a bit, both for auto-completion, and for getting ideas on how to implement certain features.
