#ifndef OPENGLPROGRAM_TEXTURED_CUBE_H
#define OPENGLPROGRAM_TEXTURED_CUBE_H

#endif //OPENGLPROGRAM_TEXTURED_CUBE_H

#ifndef TEXTURED_CUBE_H
#define TEXTURED_CUBE_H

#include "cube.h"
#include "texture.h"

class TexturedCube : public Cube {
public:
    TexturedCube(Shader* shader_program, Texture* texture);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Texture* texture;
    GLint loc_diffuse_map;
};

#endif // TEXTURED_CUBE_H
