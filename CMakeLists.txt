cmake_minimum_required(VERSION 3.20)

project(OpenGLProgram)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Source / include directories
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Global include directory for your own headers
include_directories(${INCLUDE_DIR})

# Source files
set(SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/node.cpp
        ${SRC_DIR}/shape.cpp
        ${SRC_DIR}/shader.cpp
        ${SRC_DIR}/viewer.cpp
        ${SRC_DIR}/triangle.cpp
        ${SRC_DIR}/pyramid.cpp
        ${SRC_DIR}/cylinder.cpp
        ${SRC_DIR}/sphere.cpp
        ${SRC_DIR}/texture.cpp
        ${SRC_DIR}/textured_sphere.cpp
        ${SRC_DIR}/lighting_sphere.cpp
        ${SRC_DIR}/maze.cpp
        ${SRC_DIR}/cube.cpp
        ${SRC_DIR}/textured_cube.cpp
        src/sky_sphere.cpp
)

# Assets paths available in code
add_compile_definitions(SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")
add_compile_definitions(TEXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/textures/")

# Dependencies
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
)

FetchContent_Declare(
        glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG glew-cmake-2.2.0
)

FetchContent_MakeAvailable(glfw glm glew)

# Executable
add_executable(opengl_program ${SOURCES})

target_include_directories(opengl_program PRIVATE ${INCLUDE_DIR})

target_link_libraries(opengl_program PRIVATE
        glfw
        libglew_static
        glm::glm
        opengl32
)