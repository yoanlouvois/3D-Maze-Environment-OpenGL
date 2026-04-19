# Procedural Maze (OpenGL / C++)

A simple first-person maze game built with OpenGL in C++.

The maze is generated procedurally, and the player must explore it and find the exit.

---

## Features

- Procedural maze generation
- First-person camera (FPS-style movement)
- Basic collision system
- OpenGL rendering (GLFW + GLEW + GLM)

---

## Maze overview

<p align="center">
  <img src="https://github.com/user-attachments/assets/0da72971-a2c8-43a9-8d09-bedb6da6f0c5" width="30%" />
  <img src="https://github.com/user-attachments/assets/0db15b02-48e4-476f-94dd-cdc02b5c74c9" width="30%" />
  <img src="https://github.com/user-attachments/assets/8b233161-05be-4242-93b0-09dab47994cd" width="30%" />
</p>


---

## Requirements

- C++
- CMake
- OpenGL
- Git (for submodules)

---

## Setup

Clone the repository and initialize dependencies:

```bash
git clone <your-repo-url>
cd <repo>
git submodule update --init --recursive
