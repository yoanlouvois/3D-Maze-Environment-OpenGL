#include "maze.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

Maze::Maze(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<bool>(width, true));
}

void Maze::generate() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    generateMaze(1, 1);
    createEntranceAndExit();
}

void Maze::generateMaze(int x, int y) {
    static const int dx[] = { 1, -1, 0, 0 };
    static const int dy[] = { 0, 0, 1, -1 };

    int directions[] = { 0, 1, 2, 3 };
    std::shuffle(directions, directions + 4, std::mt19937(std::random_device()()));

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[directions[i]] * 2;
        int ny = y + dy[directions[i]] * 2;

        if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1 && grid[ny][nx]) {
            grid[ny][nx] = false;
            grid[ny - dy[directions[i]]][nx - dx[directions[i]]] = false;
            generateMaze(nx, ny);
        }
    }
}

void Maze::createEntranceAndExit() {
    entrance = glm::ivec2(0, 1);  // Position de l'entrée (en bas à gauche)
    exit = glm::ivec2(width - 1, height - 2);  // Position de la sortie (en haut à droite)

    grid[entrance.y][entrance.x] = false;
    grid[exit.y][exit.x] = false;
}

glm::ivec2 Maze::getEntrancePosition() const {
    return entrance;
}

glm::ivec2 Maze::getExitPosition() const {
    return exit;
}

const std::vector<std::vector<bool>>& Maze::getGrid() const {
    return grid;
}

void Maze::addToScene(Node* scene_root, Shader* texture_shader, Texture* wall_texture, Texture* floor_texture, Shader* phong_shader) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x]) {
                // Ajouter les murs
                Shape* wall = new TexturedCube(texture_shader, wall_texture);
                glm::mat4 wall_mat = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, y))
                                     * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                Node* wall_node = new Node(wall_mat);
                wall_node->add(wall);
                scene_root->add(wall_node);

                // Ajouter une source de lumière accrochée au mur
                /**
                Shape* light = new TexturedCube(phong_shader, wall_texture);
                glm::mat4 light_mat = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.5f, y))
                                      * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
                Node* light_node = new Node(light_mat);
                light_node->add(light);
                scene_root->add(light_node);*/
            }
            // Ajouter le sol
            Shape* floor = new TexturedCube(texture_shader, floor_texture);
            glm::mat4 floor_mat = glm::translate(glm::mat4(1.0f), glm::vec3(x, -1.0f, y)) // la hauteur du sol est -1
                                  * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
            Node* floor_node = new Node(floor_mat);
            floor_node->add(floor);
            scene_root->add(floor_node);
        }
    }
}