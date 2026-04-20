#include "viewer.h"
#include "texture.h"
#include "maze.h"
#include "textured_cube.h"
#include "sky_sphere.h"
#include "planet_sphere.h"
#include <string>
#include <random>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

#ifndef TEXTURE_DIR
#error "TEXTURE_DIR not defined"
#endif

int main()
{
    // Création de la fenêtre et initialisation du Viewer
    Viewer viewer;

    // Obtention du répertoire des shaders
    std::string shader_dir = SHADER_DIR;

    // Chargement du shader pour les textures
    Shader *texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");
    Shader *phong_shader = new Shader(shader_dir + "phong.vert", shader_dir + "phong.frag");
    Shader *textured_phong_shader = new Shader(shader_dir + "textured_phong.vert", shader_dir + "textured_phong.frag");

    // Mettre le shader sur le viewer
    viewer.setLightingShader(textured_phong_shader);

    // Obtention du répertoire des textures
    std::string texture_dir = TEXTURE_DIR;

    // Chargement de la texture des murs du labyrinthe
    Texture *wall_texture = new Texture(texture_dir + "wall1.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Texture *floor_texture = new Texture(texture_dir + "floor.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Texture *start_texture = new Texture(texture_dir + "start.jpg", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Texture *end_texture = new Texture(texture_dir + "youWin.jpg", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // Chargement de la texture du ciel
    Texture *sky_texture = new Texture(texture_dir + "sky2.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    //Chargement de la texture des planètes
    Texture *planet_texture = new Texture(texture_dir + "planet.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // Définition de la taille du labyrinthe
    int maze_width = 21;
    int maze_height = 21;

    // Génération du labyrinthe
    Maze maze(maze_width, maze_height);
    maze.generate();

    // Positionnement initial de la caméra à l'entrée du labyrinthe
    glm::vec3 entrancePosition = glm::vec3(maze.getEntrancePosition().x, 0.0f, maze.getEntrancePosition().y);
    viewer.setInitialCameraPosition(entrancePosition + glm::vec3(1.0f, 0.0f, 0.0f));
    //glm::vec3 exitPosition = glm::vec3(maze.getExitPosition().x, 0.0f, maze.getExitPosition().y);


    // Mise à jour de la grille de murs dans le Viewer
    viewer.setWallGrid(maze.getGrid());

    // Ajout des murs du labyrinthe à la scène du Viewer
    maze.addToScene(viewer.scene_root, textured_phong_shader, wall_texture, floor_texture, phong_shader);

    // Création de la sphère du ciel
    SkySphere* sky = new SkySphere(texture_shader, sky_texture, 48, 24);
    glm::vec3 skyCenter = glm::vec3(maze_width / 2.0f, 6.0f, maze_height / 2.0f);
    // scale négatif sur X pour voir l'intérieur de la sphère
    glm::mat4 sky_mat = glm::translate(glm::mat4(1.0f), skyCenter)
                        * glm::scale(glm::mat4(1.0f), glm::vec3(-80.0f, 40.0f, 80.0f));

    Node* sky_node = new Node(sky_mat);
    sky_node->add(sky);
    viewer.scene_root->add(sky_node);


    // Configuration de l'éclairage
    phong_shader->use();
    phong_shader->setVec3("lightColor", glm::vec3(0.45f, 0.55f, 0.9f));
    phong_shader->setVec3("lightPos", glm::vec3(10.0f, 3.0f, 10.0f));

    // Eclairage des murs
    textured_phong_shader->use();
    textured_phong_shader->setVec3("lightColor", glm::vec3(0.45f, 0.55f, 0.9f));
    textured_phong_shader->setVec3("lightPos", glm::vec3(10.0f, 3.0f, 10.0f));

    // Ajouter un cube au début du labyrinthe
    glm::ivec2 positionEntree = maze.getEntrancePosition();
    glm::vec3 startCubePosition = glm::vec3(positionEntree.x, 0.0f, positionEntree.y);

    TexturedCube* startCube = new TexturedCube(texture_shader, start_texture);
    glm::mat4 startCube_mat = glm::translate(glm::mat4(1.0f), startCubePosition)
                        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f))  // Rotation de 180 degrés autour de l'axe x
                         * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Node* startCube_node = new Node(startCube_mat);
    startCube_node->add(startCube);
    viewer.scene_root->add(startCube_node);


    // Ajouter un cube à la sortie du labyrinthe
    glm::ivec2 exitPosition = maze.getExitPosition();
    glm::vec3 cubePosition = glm::vec3(exitPosition.x, 0.0f, exitPosition.y);

    TexturedCube* exitCube = new TexturedCube(texture_shader, end_texture);
    glm::mat4 cube_mat = glm::translate(glm::mat4(1.0f), cubePosition)
                        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f))  // Rotation de 180 degrés autour de l'axe x
                         * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Node* cube_node = new Node(cube_mat);
    cube_node->add(exitCube);
    viewer.scene_root->add(cube_node);

    //Planètes
    // Ajout de 5 planètes aléatoires au-dessus du labyrinthe
    std::mt19937 rng(std::random_device{}());

    // bornes de placement
    std::uniform_real_distribution<float> distX(-10.0f, maze_width + 10.0f);
    std::uniform_real_distribution<float> distY(6.0f, 14.0f);
    std::uniform_real_distribution<float> distZ(-10.0f, maze_height + 10.0f);
    std::uniform_real_distribution<float> distScale(1.0f, 3.0f);
    std::uniform_real_distribution<float> distRot(0.0f, 360.0f);

    for (int i = 0; i < 100; ++i) {
        PlanetSphere* planet = new PlanetSphere(texture_shader, planet_texture, 24, 16);

        glm::vec3 planetPos(
                distX(rng),
                distY(rng),
                distZ(rng)
        );

        float scale = distScale(rng);
        float angle = glm::radians(distRot(rng));

        glm::mat4 planet_mat = glm::translate(glm::mat4(1.0f), planetPos)
                               * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f))
                               * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

        Node* planet_node = new Node(planet_mat);
        planet_node->add(planet);
        viewer.scene_root->add(planet_node);
    }

    // Lancement de la boucle principale de rendu du Viewer
    viewer.run();

    return 0;
}