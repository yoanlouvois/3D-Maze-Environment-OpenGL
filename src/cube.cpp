#include "cube.h"
#include <GL/glew.h>

Cube::Cube(Shader* shader_program) : Shape(shader_program) {
    buildVertices();
    buildIndices();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(4, &buffers[0]);

    // Position attribute
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Normal attribute
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    num_indices = static_cast<unsigned int>(indices.size());
}


Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(3, buffers);
}

void Cube::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);

    glBindVertexArray(VAO);

    Shape::draw(model, view, projection);

    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
}

void Cube::buildVertices() {
    vertices = {
            // Front face
            glm::vec3(-0.5f, -0.5f,  0.5f), // Vertex 1
            glm::vec3( 0.5f, -0.5f,  0.5f), // Vertex 2
            glm::vec3( 0.5f,  0.5f,  0.5f), // Vertex 3
            glm::vec3(-0.5f,  0.5f,  0.5f), // Vertex 4
            // Back face
            glm::vec3(-0.5f, -0.5f, -0.5f), // Vertex 5
            glm::vec3( 0.5f, -0.5f, -0.5f), // Vertex 6
            glm::vec3( 0.5f,  0.5f, -0.5f), // Vertex 7
            glm::vec3(-0.5f,  0.5f, -0.5f)  // Vertex 8
    };

    normals = {
            glm::vec3( 0.0f,  0.0f,  1.0f),
            glm::vec3( 0.0f,  0.0f,  1.0f),
            glm::vec3( 0.0f,  0.0f,  1.0f),
            glm::vec3( 0.0f,  0.0f,  1.0f),
            glm::vec3( 0.0f,  0.0f, -1.0f),
            glm::vec3( 0.0f,  0.0f, -1.0f),
            glm::vec3( 0.0f,  0.0f, -1.0f),
            glm::vec3( 0.0f,  0.0f, -1.0f)
    };

    texCoords = {
            // Front face
            glm::vec2(0.0f, 0.0f), // Vertex 1
            glm::vec2(1.0f, 0.0f), // Vertex 2
            glm::vec2(1.0f, 1.0f), // Vertex 3
            glm::vec2(0.0f, 1.0f), // Vertex 4
            // Back face
            glm::vec2(1.0f, 0.0f), // Vertex 5
            glm::vec2(0.0f, 0.0f), // Vertex 6
            glm::vec2(0.0f, 1.0f), // Vertex 7
            glm::vec2(1.0f, 1.0f)  // Vertex 8
    };
}



void Cube::buildIndices() {
    indices = {
            0, 1, 2, 2, 3, 0, // Front face
            4, 5, 6, 6, 7, 4, // Back face
            0, 1, 5, 5, 4, 0, // Bottom face
            2, 3, 7, 7, 6, 2, // Top face
            0, 3, 7, 7, 4, 0, // Left face
            1, 2, 6, 6, 5, 1  // Right face
    };
}
