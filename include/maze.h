#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include "node.h"
#include "shader.h"
#include "texture.h"
#include "textured_cube.h"

class Maze {
public:
    Maze(int width, int height);
    void generate();
    void addToScene(Node* scene_root, Shader* texture_shader, Texture* wall_texture, Texture* floor_texture, Shader* phong_shader);
    glm::ivec2 getEntrancePosition() const;
    glm::ivec2 getExitPosition() const;
    const std::vector<std::vector<bool>>& getGrid() const;

private:
    int width, height;
    std::vector<std::vector<bool>> grid;
    void generateMaze(int x, int y);
    void createEntranceAndExit();
    glm::ivec2 entrance, exit;
};

#endif // MAZE_H