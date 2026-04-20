#ifndef PLANET_SPHERE_H
#define PLANET_SPHERE_H

#include "shape.h"
#include "texture.h"
#include <vector>
#include <glm/glm.hpp>

class PlanetSphere : public Shape {
public:
    PlanetSphere(Shader* shader_program, Texture* texture, int sector_count = 24, int stack_count = 16);
    ~PlanetSphere();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Texture* texture;
    GLuint VAO;
    GLuint buffers[4];
    GLuint loc_diffuse_map;
    unsigned int num_indices;

    int sector_count;
    int stack_count;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;

    void buildVertices();
    void buildIndices();
};

#endif