#include "viewer.h"
#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

// Constructeur : initialise les paramètres de la fenêtre et de la caméra
Viewer::Viewer(int width, int height)
        : yaw(-90.0f), pitch(0.0f), lastX(width / 2.0f), lastY(height / 2.0f), firstMouse(true), cameraSpeed(0.01f),
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
    //win = glfwCreateWindow(width, height, "Viewer", NULL, NULL);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    win = glfwCreateWindow(mode->width, mode->height, "Viewer", monitor, NULL);

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
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);

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
    // On nettoie la boucle : une seule boucle run
    while (!glfwWindowShouldClose(win))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Gestion des entrées et du mouvement
        handle_camera_movement();

        // Calcul des matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1024/768, 0.1f, 100.0f);

        if (lightingShader) {
            lightingShader->use();
            lightingShader->setVec3("lightPos", cameraPos);
        }

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
    float radius = 0.15f;

    std::vector<glm::vec3> points = {
            glm::vec3(newPos.x - radius, newPos.y, newPos.z - radius),
            glm::vec3(newPos.x + radius, newPos.y, newPos.z - radius),
            glm::vec3(newPos.x - radius, newPos.y, newPos.z + radius),
            glm::vec3(newPos.x + radius, newPos.y, newPos.z + radius)
    };

    for (const auto& p : points) {
        int x = static_cast<int>(p.x + 0.5f);
        int z = static_cast<int>(p.z + 0.5f);

        if (x < 0 || x >= static_cast<int>(wallGrid[0].size()) ||
            z < 0 || z >= static_cast<int>(wallGrid.size())) {
            return true;
        }

        if (wallGrid[z][x]) {
            return true;
        }
    }

    return false;
}

// Gestion du mouvement de la caméra
void Viewer::handle_camera_movement() {
    float currentSpeed = this->cameraSpeed; // Utilise ta variable membre
    if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        currentSpeed = 0.02f;
    }

    // 1. Calcul du vecteur de déplacement horizontal (XZ uniquement)
    glm::vec3 walkFront = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    glm::vec3 walkRight = glm::normalize(glm::cross(walkFront, cameraUp));
    glm::vec3 moveDir = glm::vec3(0.0f);

    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) moveDir += walkFront;
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) moveDir -= walkFront;
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) moveDir -= walkRight;
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) moveDir += walkRight;

    // 2. Application du déplacement avec collision
    if (glm::length(moveDir) > 0) {
        moveDir = glm::normalize(moveDir) * currentSpeed;

        glm::vec3 newPos = cameraPos;

        // Test déplacement sur X uniquement
        glm::vec3 tryX = newPos;
        tryX.x += moveDir.x;
        if (!checkCollision(tryX)) {
            newPos.x = tryX.x;
        }

        // Test déplacement sur Z uniquement
        glm::vec3 tryZ = newPos;
        tryZ.z += moveDir.z;
        if (!checkCollision(tryZ)) {
            newPos.z = tryZ.z;
        }

        cameraPos.x = newPos.x;
        cameraPos.z = newPos.z;
    }

    // 3. Effet de marche amélioré
    apply_head_bobbing();
}

void Viewer::apply_head_bobbing() {
    // On utilise le temps global de GLFW pour ne pas dépendre du deltaTime en paramètre
    float time = (float)glfwGetTime();

    // Paramètres réglables pour le "feeling"
    float bobFreq = 10.0f;     // Fréquence des pas
    float bobMoveAmp = 0.05f;  // Amplitude verticale
    float sideStepAmp = 0.03f; // Amplitude latérale

    bool isMoving = (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS ||
                     glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS ||
                     glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS ||
                     glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS);

    if (isMoving) {
        // Courbe en "M" pour le vertical (plus naturel qu'un simple sinus)
        // sin(x) donne un balancement, abs(sin(x)) simule l'impact du pied
        float waveY = std::abs(std::sin(time * bobFreq)) * bobMoveAmp;

        // Balancement latéral (oscille deux fois moins vite que le pas vertical)
        float waveX = std::sin(time * bobFreq * 0.5f) * sideStepAmp;

        // On applique la hauteur de base + l'oscillation
        cameraPos.y = fixedHeight + waveY;

        // Pour le balancement latéral, on décale légèrement la caméra sur son axe local "droite"
        //glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
        //cameraPos += right * waveX * 0.1f; // Influence légère pour ne pas donner le mal de mer
    } else {
        // Au repos : retour fluide à la hauteur fixe
        cameraPos.y = glm::mix(cameraPos.y, fixedHeight, 0.1f);
    }
}

glm::vec3 Viewer::getCameraPosition() const {
    return cameraPos;
}

void Viewer::setLightingShader(Shader* shader) {
    lightingShader = shader;
}


