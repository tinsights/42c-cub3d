# Cub3D

## Overview

Cub3D is a simple raycasting engine implemented in C, inspired by classic first-person shooter games like Wolfenstein 3D. This project demonstrates the principles of 3D rendering using raycasting techniques, allowing users to explore a 3D environment created from 2D map data.

## Features

- **Raycasting Engine**: Utilizes raycasting to render a 3D perspective from a 2D map.
- **Texture Mapping**: Supports texture mapping for walls and surfaces, enhancing visual realism.
- **User Interaction**: Allows user movement and camera rotation within the 3D space.
- **Basic Controls**: Navigate using keyboard inputs for movement and viewing angles.
- **MinilibX Integration**: Leverages the MinilibX graphics library for rendering.

## Requirements

To build and run Cub3D, you will need:

- A C compiler (e.g., gcc)
- MinilibX graphics library
- Basic understanding of C programming and graphics rendering concepts

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/tinsights/cub3d.git
   cd cub3d
   ```

2. **Install MinilibX**:
   Follow the instructions in the MinilibX documentation to install the library on your system.

3. **Build the Project**:
   ```bash
   make
   ```

4. **Run the Game**:
   ```bash
   ./cub3d map.cub
   ```

## Usage

Once the application is running, you can explore the 3D environment using the following controls:

- **W**: Move forward
- **S**: Move backward
- **A**: Move left
- **D**: Move right
- **Arrow Keys / Mouse Movement**: Rotate the camera view

### Map File

The game requires a `.cub` file that defines the layout of the environment. This file includes information about walls, textures, and player starting positions.

## Contributing

Contributions are welcome! If you would like to contribute to this project, please fork the repository and submit a pull request. Feedback and suggestions for improvement are also appreciated.

## Contact

For questions or feedback, please open an issue in the GitHub repository.

--
