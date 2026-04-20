#ifndef VIEWER_H
#define VIEWER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "node.h"
#include <vector>

class Viewer {
public:
    Viewer(int width = 800, int height = 600);
    void run();
    void setInitialCameraPosition(const glm::vec3& position);
    void setWallGrid(const std::vector<std::vector<bool>>& grid);

    Node* scene_root;
private:
    GLFWwindow* win;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float yaw, pitch;
    float lastX, lastY;
    bool firstMouse;
    float cameraSpeed;
    float fixedHeight;
    std::vector<std::vector<bool>> wallGrid;
    float walkTimer = 0.0f;
    float bobAmount = 0.05f;
    float bobSpeed = 10.0f;
    float deltaTime;

    void on_key(int key, int action);
    void on_mouse_move(double xpos, double ypos);
    void handle_camera_movement();
    void apply_head_bobbing();
    bool checkCollision(const glm::vec3& newPos);
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback_static(GLFWwindow* window, double xpos, double ypos);
};

#endif // VIEWER_H
