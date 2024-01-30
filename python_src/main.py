# Import necessary libraries
import pygame
import numpy as np
import random
import itertools

# Define constants
particleSize = 10
WIDTH, HEIGHT = 1920, 1080
gridWidth, gridHeight = 108, 192

# Define the Sand class
class Sand:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.color = random.choice([(246, 215, 176), (236, 204, 162), (225, 191, 146)])

    # Define the fall method for the Sand class
    def fall(self, grid):
        if self.y < len(grid) - 1:
            positions = [(self.x, self.y + 1), (self.x - 1, self.y + 1), (self.x + 1, self.y + 1)]
            random.shuffle(positions)
            available_positions = [(x, y) for x, y in positions if
                                   0 <= x < len(grid[0]) and 0 <= y < len(grid) and grid[y][x] == 0]
            if available_positions:
                new_x, new_y = random.choice(available_positions)
                grid[self.y][self.x] = 0
                self.x = new_x
                self.y = new_y
                grid[self.y][self.x] = self

# Define the Stone class
class Stone:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.color = (150, 150, 150)  # Gray color

    # Define the update method for the Stone class
    def update(self, grid):
        pass

# List of particle types and current index
particle_types = [Sand, Stone]
current_particle_type_index = 0

# Initialize pygame and the grid
pygame.init()
window = pygame.display.set_mode((WIDTH, HEIGHT))
grid = np.zeros((gridWidth, gridHeight), dtype=object)

# Initialize brush size
brushSize = 5

# Main game loop
running = True
mouse_down = False
right_mouse_down = False
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Left mouse button
                mouse_down = True
            elif event.button == 3:  # Right mouse button
                right_mouse_down = True
        elif event.type == pygame.MOUSEBUTTONUP:
            if event.button == 1:  # Left mouse button
                mouse_down = False
            elif event.button == 3:  # Right mouse button
                right_mouse_down = False
        elif event.type == pygame.MOUSEWHEEL:
            brushSize += event.y  # Increase or decrease brush size based on scroll direction
            brushSize = max(1, min(brushSize, 20))  # Limit brush size between 1 and 10
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_TAB:  # Tab key
                # Cycle through particle types
                current_particle_type_index = (current_particle_type_index + 1) % len(particle_types)

    if mouse_down or right_mouse_down:
        mx, my = pygame.mouse.get_pos()
        mx //= particleSize  # Adjust for the particle size
        my //= particleSize
        for x in range(mx - brushSize, mx + brushSize):
            for y in range(my - brushSize, my + brushSize):
                if 0 <= x < gridHeight and 0 <= y < gridWidth:  # Check if within grid
                    dx, dy = x - mx, y - my
                    if dx * dx + dy * dy <= brushSize * brushSize:  # Check if within brush radius
                        if mouse_down:
                            # Create a particle of the current type
                            grid[y][x] = particle_types[current_particle_type_index](x, y)
                        elif right_mouse_down:
                            grid[y][x] = 0

    # Flatten the grid into a list and shuffle it
    flat_grid = list(itertools.chain(*grid))
    random.shuffle(flat_grid)

    # Update the particles in the shuffled order
    for cell in flat_grid:
        if isinstance(cell, Sand):
            cell.fall(grid)

    # Draw the grid
    window.fill((0, 0, 0))
    for y, row in enumerate(grid):
        for x, cell in enumerate(row):
            if isinstance(cell, Sand) or isinstance(cell, Stone):
                pygame.draw.rect(window, cell.color,
                                 pygame.Rect(x * particleSize, y * particleSize, particleSize, particleSize))

    # Draw the brush outline
    pygame.draw.circle(window, (255, 255, 255), pygame.mouse.get_pos(), brushSize * particleSize, 1)

    pygame.display.flip()

pygame.quit()