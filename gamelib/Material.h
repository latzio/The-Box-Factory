#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "Algebra.h"
#include <GL/gl.h>

class Material {
public:
  virtual ~Material();
  virtual void apply_gl() const = 0;
  enum Type{ PHONG, TEXTURE };
  virtual Type get_type() = 0;


protected:
  Material()
  {
  }
};

class PhongMaterial : public Material {
public:
  PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
  virtual ~PhongMaterial();

  virtual Type get_type() {return PHONG; }

  virtual void apply_gl() const;

  Colour m_kd;
  Colour m_ks;

private:
  double m_shininess;
};

class TextureMaterial : public PhongMaterial {
public:
    TextureMaterial(const Colour& kd, const Colour& ks, double shininess, int nTextureIndex);
    virtual ~TextureMaterial();

    virtual Type get_type() {return TEXTURE; }

    virtual void apply_gl() const;

private:
    int m_nTextureIndex;
};

#endif
