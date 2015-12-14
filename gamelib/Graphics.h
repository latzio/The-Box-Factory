#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "Macro.h"

#include <glm/glm.hpp>

constexpr int UniformCount = 5;
enum class Uniform {
    Modelview,
    ModelviewInverseTranspose,
    Perspective,
    Texture,
    Color,
};

constexpr int ShaderProgramCount = 2;
enum class ShaderProgram {
    Texture,
    Color,
};

enum class Geometry {
    Sphere,
    Plane,
    Cube,
};

class Graphics {
    public:
        Graphics();
        ~Graphics();
        NoMoveOrCopy(Graphics)

        void initialize();
        void useProgram(ShaderProgram);

        void setUniformMatrix(Uniform, const glm::mat4&);
        void applyUniforms();

        void draw(Geometry);

        int m_shaderPrograms[ShaderProgramCount];
        int a_tex[ShaderProgramCount];
        int a_normal[ShaderProgramCount];
        int a_position[ShaderProgramCount];
        int u_perspective[ShaderProgramCount];
        int u_modelview[ShaderProgramCount];
        int u_modelview_ivt[ShaderProgramCount];
        int u_color[ShaderProgramCount];
        int u_texture[ShaderProgramCount];
        int u_shininess[ShaderProgramCount];

        int m_programInUse;
        glm::mat4 m_perspectiveMatrix;
        glm::mat4 m_modelviewMatrix;
        glm::mat4 m_modelviewMatrixIvt;
};

#endif
