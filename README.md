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
  <img src="https://github.com/user-attachments/assets/9079ad43-194d-4d79-afbb-cc5b618aa14d" width="30%" />
  <img src="https://github.com/user-attachments/assets/94b1438b-cedd-4734-a4d6-dab3429da794" width="30%" />
  <img src="https://github.com/user-attachments/assets/9a986cc8-fa0c-426b-8495-c724f2c581f0" width="30%" />
</p>

---

## Requirements

- A C++ compiler with C++17 support
- CMake 3.20 or newer
- OpenGL
- Git
- Internet connection (dependencies are fetched automatically by CMake)

---

## Setup

### Option 1 — Build manually with CMake

Clone the repository:

```bash
git clone <your-repo-url>
cd <repo>
```

Configure the project:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

Build it:

```bash
cmake --build build
```

Run the executable:

- **Windows**
```bash
.\build\opengl_program.exe
```

- **Linux / macOS**
```bash
./build/opengl_program
```

---

### Option 2 — Open and run with CLion

Clone the repository:

```bash
git clone <your-repo-url>
cd <repo>
```

Then:

1. Open the project folder in **CLion**
2. Let CLion load the `CMakeLists.txt`
3. Wait for the first CMake configure step to finish  
   (CMake will automatically download GLFW, GLEW, and GLM)
4. Select the `opengl_program` target
5. Click **Run** or **Debug**

If CLion has issues with an old configuration, delete the build directory and reload:

- On Linux / macOS:
```bash
rm -rf cmake-build-debug
```

- On Windows: delete the `cmake-build-debug` folder manually

---

### Notes

- No need to initialize git submodules
- Dependencies are handled automatically via CMake (`FetchContent`)
- Do not commit build folders (`build/`, `cmake-build-debug/`)
- Do not commit generated dependency folders (`_deps/`, `*-src`, `*-build`)
