# Space Shooter

A 2D space shooter game built with SDL2 where players control a spaceship, shoot enemies, and try to survive as long as possible.

## Game Features

- Smooth spaceship movement with WASD controls
- Mouse-aimed shooting mechanics
- Multiple enemy types with different behaviors
- Parallax scrolling background for depth effect
- Health system with visual health bar
- Scoring system that tracks player progress
- Sound effects for shooting
- Game over screen with final score and restart option

## Controls

- **W, A, S, D**: Move the spaceship (up, left, down, right)
- **Mouse**: Aim your weapons
- **Left Mouse Button**: Shoot
- **Enter**: Restart after game over

## Game Mechanics

- Enemies spawn around the player and follow the player's position
- Enemies shoot bullets at the player periodically
- Player loses health when hit by enemy bullets
- Destroying enemies awards points (5 points per hit, 10 bonus points for destroying)
- Game ends when player health reaches zero

## Technical Details

The game is built using:
- SDL2 for rendering and input handling
- SDL2_image for texture loading
- SDL2_mixer for sound effects
- SDL2_ttf for text rendering

The codebase is organized into several key components:
- Player management (movement, rendering, health)
- Enemy system (AI, rendering, spawning)
- Bullet physics (movement, collision detection)
- Background rendering with parallax scrolling and **tile rendering**
- Health bar and UI elements
- **Coordinate Systems:** The game utilizes two coordinate systems:
    - **Game/World Coordinates:** Represents the absolute position of objects within the game world.
    - **Camera/Screen Coordinates:** Represents what is currently visible on the screen. The camera follows the player, translating world coordinates to screen coordinates for rendering.
- **Physics:** Basic physics principles are applied for smooth player and object movement. This also handles the transformation between the game world and camera coordinates.

## Building and Running

### Prerequisites

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf
- C++ compiler (GCC recommended)
- Code::Blocks IDE (project files included)

### How to Build

1. Open the `space_game.cbp` file in Code::Blocks
2. Select the build target (Debug or Release)
3. Build the project (F9)

### Running the Game

Ensure all the DLL files and resource folder are in the same directory as the executable:
- SDL2.dll
- SDL2_image.dll
- SDL2_mixer.dll
- SDL2_ttf.dll
- resources/ folder (contains all game assets)

