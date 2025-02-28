#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

/** \brief A graphic program.*/
class Shader {
public:
    /** \brief the shader constructor. Should never be called alone (use loader functions)*/
    Shader(const std::string& vertex_path, const std::string& fragment_path);

    /* \brief Destructor. Destroy the shader component created */
    ~Shader();

    GLuint get_id();
    void use();
    void setVec3(const std::string &name, const glm::vec3 &value) const;

private:
    GLuint glid;
    GLuint compile_shader(const std::string& path, GLenum shader_type);
};

#endif // SHADER_H