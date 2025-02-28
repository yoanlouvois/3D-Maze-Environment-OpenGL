#ifndef CUBE_H
#define CUBE_H

#include "shape.h"

class Cube : public Shape {
public:
    Cube(Shader* shader_program);
    ~Cube();
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

protected:
    void buildVertices();
    void buildIndices();
    GLuint VAO, buffers[4]; // Mise à jour du nombre de buffers
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords; // Ajout du vecteur pour les coordonnées de texture
    std::vector<unsigned int> indices;
    unsigned int num_indices;
};

#endif // CUBE_H
