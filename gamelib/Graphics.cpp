#include "Graphics.h"

#include "Image.h"
#include "Textures.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <glm/gtx/string_cast.hpp>

#include <GLES3/gl3.h>

#include <fstream>
#include <iostream>

using namespace glm;


static const char* vsPath[] = { "common.vs.glsl", "common.vs.glsl" };
static const char* fsPath[] = { "texture.fs.glsl", "color.fs.glsl" };

static void print_info_log(GLuint  name)
{
    constexpr int maxLen = 2024;
    int len = 0;
    char buffer[maxLen];

    if (glIsProgram(name)) {
        glGetProgramInfoLog(name , 2014 , &len , buffer);
        std::cout << "Program " << name << " Info: " <<  buffer << std::endl;
    } else {
        glGetShaderInfoLog(name , 2014 , &len , buffer);
        if (len > 0) {
            std::cout << "Shader Info: " <<  buffer << std::endl;

            GLint success;
            glGetShaderiv(name, GL_COMPILE_STATUS, &success);
            if (success != GL_TRUE)   exit(1);
        }
    }
}

static GLuint load_shader(const char* shader_file, GLenum type)
{
    std::string glslDir = "glsl/";
    std::string path = glslDir + shader_file;
    std::ifstream t(path);

    const std::string src((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    const char* srcArray = &src[0];

    GLuint  shader = glCreateShader(type);

    glShaderSource(shader, 1, &srcArray, 0);
    glCompileShader(shader);
    print_info_log(shader);

    return shader;
}

Graphics::Graphics()
    : m_programInUse(-1)
    , m_perspectiveMatrix()
    , m_modelviewMatrix()
    , m_modelviewMatrixIvt()
{
}

Graphics::~Graphics()
{
}

void Graphics::initialize()
{
    // Texture mapping
    std::cout << "Textures: " << TEXTURE_COUNT << std::endl;
    glEnable(GL_TEXTURE_2D);

    Image image;
    for (int c = 1; c < TEXTURE_COUNT; c++) {

        // Load into OpenGL
        image.loadPng(Textures::texFolder + Textures::texPaths[ c ]);
        std::cout << "Loading texture from: " << Textures::texFolder + Textures::texPaths[ c ] << std::endl;
        glBindTexture(GL_TEXTURE_2D, Textures::TEX_NO_TEXTURE + c);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                     0, (image.elements() == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, image.byteData());

    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    for (int i = 0; i < ShaderProgramCount; ++i) {
        m_shaderPrograms[i]  = glCreateProgram();                  // create program object

        GLuint vertexShader = load_shader(vsPath[i], GL_VERTEX_SHADER);         // load vertex shader
        glAttachShader(m_shaderPrograms[i], vertexShader);                // and attach both...

        GLuint fragmentShader = load_shader(fsPath[i], GL_FRAGMENT_SHADER);     // load fragment shader
        glAttachShader(m_shaderPrograms[i], fragmentShader);              // ... shaders to it

        glLinkProgram(m_shaderPrograms[i]);       // link the program
        print_info_log(m_shaderPrograms[i]);

        int linkStatus = 0;
        glGetProgramiv(m_shaderPrograms[i], GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE)
            std::cerr << "Failed to link." << std::endl;
        //// now get the locations (kind of handle) of the shaders variables
        u_perspective[i] = glGetUniformLocation(m_shaderPrograms[i] , "u_perspective");
        u_modelview[i] = glGetUniformLocation(m_shaderPrograms[i] , "u_modelview");
        u_modelview_ivt[i] = glGetUniformLocation(m_shaderPrograms[i] , "u_modelview_ivt");
        u_texture[i] = glGetUniformLocation(m_shaderPrograms[i] , "u_texture");
        u_color[i] = glGetUniformLocation(m_shaderPrograms[i] , "u_color");
        u_shininess[i] = glGetUniformLocation(m_shaderPrograms[i] , "u_shininess");
        a_position[i] = glGetAttribLocation(m_shaderPrograms[i] , "a_position");
        a_normal[i] = glGetAttribLocation(m_shaderPrograms[i] , "a_normal");
        a_tex[i] = glGetAttribLocation(m_shaderPrograms[i] , "a_tex");

        std::cerr << "ProgramName " << m_shaderPrograms[i]
                  << ", a_tex " << a_tex[i]
                  << ", a_normal " << a_normal[i]
                  << ", a_pos " << a_position[i]
                  << ", u_pers " << u_perspective[i]
                  << ", u_mv " << u_modelview[i]
                  << ", u_mv_ivt " << u_modelview_ivt[i]
                  << ", u_color " << u_color[i]
                  << ", u_tex " << u_texture[i] << std::endl;

        if (a_position[i] >= 0) {
            glEnableVertexAttribArray(a_position[i]);
            glVertexAttribPointer(a_position[i], 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
        }
        if (a_normal[i] >= 0) {
            glEnableVertexAttribArray(a_normal[i]);
            glVertexAttribPointer(a_normal[i], 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        }
        if (a_tex[i] >= 0) {
            glEnableVertexAttribArray(a_tex[i]);
            glVertexAttribPointer(a_tex[i], 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
        }
        if (u_texture[i] >= 0) {
            glUniform1i(u_texture[i], 0);
        }
    }

    useProgram(ShaderProgram::Texture);
}

void Graphics::useProgram(ShaderProgram program)
{
    if (m_programInUse != static_cast<int>(program)) {
        m_programInUse = static_cast<int>(program);
        glUseProgram(m_shaderPrograms[m_programInUse]);
    }
}

void Graphics::setUniformMatrix(Uniform uniform, const glm::mat4& m)
{
    switch (uniform) {
    case Uniform::Modelview:
        m_modelviewMatrix = m;
        m_modelviewMatrixIvt = inverseTranspose(m);
        break;
    case Uniform::ModelviewInverseTranspose:
        m_modelviewMatrixIvt = m;
        m_modelviewMatrix = inverseTranspose(m);
        break;
    case Uniform::Perspective:
        m_perspectiveMatrix = m;
        break;
    case Uniform::Texture:
    case Uniform::Color:
        break;
    }

}

void Graphics::applyUniforms()
{
    glUniformMatrix4fv(u_perspective[m_programInUse], 1, GL_FALSE, value_ptr(m_perspectiveMatrix));
    glUniformMatrix4fv(u_modelview[m_programInUse], 1, GL_FALSE, value_ptr(m_modelviewMatrix));
    glUniformMatrix4fv(u_modelview_ivt[m_programInUse], 1, GL_FALSE, value_ptr(m_modelviewMatrixIvt));
}

void Graphics::draw(Geometry geometry)
{

    switch (geometry) {
    case Geometry::Sphere:
        glDrawArrays(GL_TRIANGLES, 0, 72);
        break;
    case Geometry::Plane:
        glDrawArrays(GL_TRIANGLES, 72, 6);
        break;
    case Geometry::Cube:
        glDrawArrays(GL_TRIANGLES, 78, 36);
        break;
    }
}

