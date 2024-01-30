import pygame # The graphics library used to render everything.
import numpy as np # A library containing functions for working with iterables. (meaning lists, tuples, etc.)
import itertools # Another library containing useful functions for working with iterables.
import random # A library containing functions for generating random numbers.

# The size of each particle in pixels. The smaller the particle size, the more particles there are.
particleSize = 10

# The size of the window in pixels.
WIDTH, HEIGHT = 1920, 1080

# The size of the grid in particles. The larger the grid, the more particles there are.
gridWidth, gridHeight = 108, 192

# The sand particle. Falls down and piles up.
class Sand:
    # Initialization function. Called when the particle is created.
    def __init__(self, x, y):
        self.x = x # Sets this particle's x position to the x parameter.
        self.y = y # Sets this particle's y position to the y parameter.
        self.color = random.choice([(246, 215, 176), (236, 204, 162), (225, 191, 146)]) # Sets this particle's color to a random color from the list. Used to make the sand look more natural.

    # Update function. Called every frame.
    def update(self, grid):
        # If the particle is not at the bottom of the grid, check if there is an empty space below it.
        if self.y < len(grid) - 1:
            positions = [(self.x, self.y + 1), (self.x - 1, self.y + 1), (self.x + 1, self.y + 1)] # The positions to check for empty spaces.
            random.shuffle(positions) # Shuffles the positions so that the sand doesn't always fall to the left or right.

            # Gets the positions that are empty and in the grid.
            available_positions = [(x, y) for x, y in positions if len(grid[0]) > x >= 0 == grid[y][x] and 0 <= y < len(grid)]

            # If there are any empty positions, move to one of them.
            if available_positions:
                new_x, new_y = random.choice(available_positions)
                grid[self.y][self.x] = 0
                self.x = new_x
                self.y = new_y
                grid[self.y][self.x] = self

# The stone particle. Does not move.
class Stone:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.color = (150, 150, 150)

    def update(self, grid):
        pass # Does nothing.

# A list of all the particle types and an index. Used to cycle through them with the TAB key.
particle_types = [Sand, Stone]
current_particle_type_index = 0

# Initializes pygame and creates the window.
pygame.init()
window = pygame.display.set_mode((WIDTH, HEIGHT))

# Creates the grid, and fills it with zeros.
grid = np.zeros((gridWidth, gridHeight), dtype=object)

# The radius of the brush in particles.
brushSize = 5

running = True
mouse_down = False
right_mouse_down = False
while running: # The main loop.
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                mouse_down = True
            elif event.button == 3:
                right_mouse_down = True
        elif event.type == pygame.MOUSEBUTTONUP:
            if event.button == 1:
                mouse_down = False
            elif event.button == 3:
                right_mouse_down = False
        elif event.type == pygame.MOUSEWHEEL:
            brushSize += event.y
            brushSize = max(1, min(brushSize, 20))
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_TAB:
                current_particle_type_index = (current_particle_type_index + 1) % len(particle_types)

    # If the mouse is down, place particles in a circle around the mouse.
    if mouse_down or right_mouse_down:
        mx, my = pygame.mouse.get_pos()
        mx //= particleSize
        my //= particleSize
        for x in range(mx - brushSize, mx + brushSize):
            for y in range(my - brushSize, my + brushSize):
                if 0 <= x < gridHeight and 0 <= y < gridWidth:
                    dx, dy = x - mx, y - my
                    if dx * dx + dy * dy <= brushSize * brushSize:
                        if mouse_down:
                            grid[y][x] = particle_types[current_particle_type_index](x, y)
                        elif right_mouse_down:
                            grid[y][x] = 0

    flat_grid = list(itertools.chain(*grid))
    random.shuffle(flat_grid)

    # Updates all the particles.
    for cell in flat_grid:
        if isinstance(cell, Sand):
            cell.update(grid)

    # Clears the window and draws the particles.
    window.fill((0, 0, 0))
    for y, row in enumerate(grid):
        for x, cell in enumerate(row):
            if isinstance(cell, Sand) or isinstance(cell, Stone):
                pygame.draw.rect(window, cell.color,
                                 pygame.Rect(x * particleSize, y * particleSize, particleSize, particleSize))

    # Draws the brush at the current mouse position.
    pygame.draw.circle(window, (255, 255, 255), pygame.mouse.get_pos(), brushSize * particleSize, 1)

    # Displays the rendered pixel buffer to the screen.
    pygame.display.flip()

# Quits pygame and exits the program.
pygame.quit()