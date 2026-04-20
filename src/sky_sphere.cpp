#include "sky_sphere.h"
#include <GL/glew.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SkySphere::SkySphere(Shader* shader_program, Texture* texture, int sector_count, int stack_count)
        : Shape(shader_program),
          texture(texture),
          VAO(0),
          num_indices(0),
          sector_count(sector_count),
          stack_count(stack_count)
{
    buildVertices();
    buildIndices();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(4, buffers);

    // positions
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // texcoords
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    num_indices = static_cast<unsigned int>(indices.size());
    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
}

SkySphere::~SkySphere() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(4, buffers);
}

void SkySphere::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture->getGLid());
    glUniform1i(loc_diffuse_map, 0);

    glBindVertexArray(VAO);

    Shape::draw(model, view, projection);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void SkySphere::buildVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();

    float radius = 0.5f;

    for (int i = 0; i <= stack_count; ++i) {
        float v = static_cast<float>(i) / static_cast<float>(stack_count);
        float stackAngle = (M_PI / 2.0f) - v * M_PI; // +pi/2 -> -pi/2

        float xy = radius * std::cos(stackAngle);
        float y = radius * std::sin(stackAngle);

        for (int j = 0; j <= sector_count; ++j) {
            float u = static_cast<float>(j) / static_cast<float>(sector_count);
            float sectorAngle = u * 2.0f * M_PI;

            float x = xy * std::cos(sectorAngle);
            float z = xy * std::sin(sectorAngle);

            vertices.emplace_back(x, y, z);
            normals.emplace_back(glm::normalize(glm::vec3(x, y, z)));
            texCoords.emplace_back(u, v);
        }
    }
}

void SkySphere::buildIndices() {
    indices.clear();

    for (int i = 0; i < stack_count; ++i) {
        int k1 = i * (sector_count + 1);
        int k2 = k1 + sector_count + 1;

        for (int j = 0; j < sector_count; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stack_count - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}