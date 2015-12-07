#include "Material.h"

#include "Graphics.h"

#include <GLES3/gl3.h>

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, float shininess)
    : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl(Graphics& gfx) const
{
    // Set colorglNormal
    // glColor3d(
    /*
    glColor3f(m_kd.R(), m_kd.G(), m_kd.B());

    // Polygon mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Ambient and diffuse lighting
    GLfloat diff[3] = { m_kd.R(), m_kd.G(), m_kd.B() };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diff);

    // Specular lighting
    GLfloat spec[3] = { m_ks.R(), m_ks.G(), m_ks.B() };
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);

    // Shininess
    glMaterialf(GL_FRONT, GL_SHININESS, m_shininess);

    // Turn off texture
    */
    gfx.useProgram(ShaderProgram::Color);
    glUniform4f(gfx.u_color[0], m_kd[0], m_kd[1], m_kd[2], 1.0f);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextureMaterial::TextureMaterial(const Colour& kd, const Colour& ks,
                                 float shininess, int nTextureId)
    : PhongMaterial(kd, ks, shininess)
    , m_nTextureIndex(nTextureId)
{
}

TextureMaterial::~TextureMaterial()
{
}

void TextureMaterial::apply_gl(Graphics& gfx) const
{
    //PhongMaterial::apply_gl(gfx);

    gfx.useProgram(ShaderProgram::Texture);
    glBindTexture(GL_TEXTURE_2D, m_nTextureIndex);
    glUniform4f(gfx.u_color[1], m_kd[0], m_kd[1], m_kd[2], 1.0f);
}
