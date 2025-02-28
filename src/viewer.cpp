#include "viewer.h"
#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

// Constructeur : initialise les paramètres de la fenêtre et de la caméra
Viewer::Viewer(int width, int height)
        : yaw(-90.0f), pitch(0.0f), lastX(width / 2.0f), lastY(height / 2.0f), firstMouse(true), cameraSpeed(0.05f),
          cameraPos(glm::vec3(0.0f, 0.0f, 3.0f)), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
          fixedHeight(0.0f)
{
    if (!glfwInit())    // initialise GLFW
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
    }

    // Version hints : créer une fenêtre GL avec OpenGL 3.3 ou supérieur et profil core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Crée la fenêtre GLFW
    win = glfwCreateWindow(width, height, "Viewer", NULL, NULL);

    if (win == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    // Rend le contexte OpenGL de la fenêtre courant
    glfwMakeContextCurrent(win);

    // Initialise GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    // Définit le pointeur utilisateur pour la fenêtre GLFW comme étant cette instance de Viewer
    glfwSetWindowUserPointer(win, this);

    // Enregistre les callbacks pour les événements clavier et les mouvements de la souris
    glfwSetKeyCallback(win, key_callback_static);
    glfwSetCursorPosCallback(win, mouse_callback_static); // Enregistre le callback de la souris
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Cache le curseur et capture la souris

    // Message utile pour vérifier les caractéristiques du renderer OpenGL
    std::cout << glGetString(GL_VERSION) << ", GLSL "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << ", Renderer "
              << glGetString(GL_RENDERER) << std::endl;

    // Initialise OpenGL en définissant le viewport et les caractéristiques de rendu par défaut
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    // Activer le test de profondeur pour le rendu 3D
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Initialise la racine de la scène graphique
    scene_root = new Node();
}

// Définit la position initiale de la caméra
void Viewer::setInitialCameraPosition(const glm::vec3& position) {
    cameraPos = position;
}

// Callback statique pour les événements clavier
void Viewer::key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_key(key, action);
}

// Callback statique pour les mouvements de la souris
void Viewer::mouse_callback_static(GLFWwindow* window, double xpos, double ypos)
{
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_mouse_move(xpos, ypos);
}

// Gestion des mouvements de la souris pour le contrôle de la caméra
void Viewer::on_mouse_move(double xpos, double ypos)
{
    // Si c'est le premier mouvement de la souris, initialise les dernières positions
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculer les décalages de la souris
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // inversé car les coordonnées y vont de bas en haut

    // Mettre à jour les dernières positions de la souris
    lastX = xpos;
    lastY = ypos;

    // Appliquer la sensibilité de la souris
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Mettre à jour les angles yaw et pitch
    yaw += xoffset;
    pitch += yoffset;

    // Contraindre le pitch pour éviter les retournements (gimbal lock)
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Calculer la nouvelle direction de la caméra
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front); // Normaliser le vecteur direction
}

void Viewer::setWallGrid(const std::vector<std::vector<bool>>& grid) {
    wallGrid = grid;
}

// Boucle principale de rendu
void Viewer::run()
{
    while (!glfwWindowShouldClose(win))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Vérifier les entrées du clavier pour le mouvement de la caméra
        handle_camera_movement();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);

        scene_root->draw(model, view, projection);

        glfwPollEvents();
        glfwSwapBuffers(win);
    }

    glfwTerminate();
}

// Gestion des événements clavier
void Viewer::on_key(int key, int action)
{
    // 'Q' ou 'Escape' pour quitter
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
    {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

bool Viewer::checkCollision(const glm::vec3& newPos) {
    // Convertir la position en indices de grille
    int x = static_cast<int>(newPos.x + 0.5f); // Arrondi à l'entier le plus proche
    int z = static_cast<int>(newPos.z + 0.5f); // Arrondi à l'entier le plus proche

    // Vérifier les limites de la grille
    if (x < 0 || x >= static_cast<int>(wallGrid[0].size()) || z < 0 || z >= static_cast<int>(wallGrid.size())) {
        return true; // Hors de la grille
    }

    // Vérifier la collision avec un mur
    return wallGrid[z][x];
}

// Gestion du mouvement de la caméra
void Viewer::handle_camera_movement() {
    float cameraSpeed = this->cameraSpeed * 0.2f; // Ajustez la vitesse en fonction du temps de frame

    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
        glm::vec3 newPos = cameraPos + cameraSpeed * cameraFront;
        if (!checkCollision(newPos)) {
            cameraPos = newPos;
        }
    }

    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
        glm::vec3 newPos = cameraPos - cameraSpeed * cameraFront;
        if (!checkCollision(newPos)) {
            cameraPos = newPos;
        }
    }

    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
        glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
        glm::vec3 newPos = cameraPos - right * cameraSpeed;
        if (!checkCollision(newPos)) {
            cameraPos = newPos;
        }
    }

    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
        glm::vec3 newPos = cameraPos + right * cameraSpeed;
        if (!checkCollision(newPos)) {
            cameraPos = newPos;
        }
    }

    // Contraindre la hauteur de la caméra
    cameraPos.y = fixedHeight;
}


