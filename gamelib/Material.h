#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include <glm/glm.hpp>

using Colour = glm::vec3;

class Graphics;

class Material {
public:
    virtual ~Material();
    virtual void apply_gl(Graphics&) const = 0;
    enum Type { PHONG, TEXTURE };
    virtual Type get_type() = 0;


protected:
    Material()
    {
    }
};

class PhongMaterial : public Material {
public:
    PhongMaterial(const Colour& kd, const Colour& ks, float shininess);
    virtual ~PhongMaterial();

    virtual Type get_type()
    {
        return PHONG;
    }

    virtual void apply_gl(Graphics&) const;

    Colour m_kd;
    Colour m_ks;

private:
    float m_shininess;
};

class TextureMaterial : public PhongMaterial {
public:
    TextureMaterial(const Colour& kd, const Colour& ks, float shininess, int nTextureIndex);
    virtual ~TextureMaterial();

    virtual Type get_type()
    {
        return TEXTURE;
    }

    virtual void apply_gl(Graphics&) const;

private:
    int m_nTextureIndex;
};

#endif
