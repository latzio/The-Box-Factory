#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include <GL/gl.h>

class Primitive {
public:
    Primitive();
    virtual ~Primitive();
    virtual void walk_gl() const = 0;
    void set_shadow(bool b)
    {
        m_bShadow = b;
    }

protected:
    bool m_bShadow;

};

class Sphere : public Primitive {
public:
    virtual ~Sphere();
    virtual void walk_gl() const;

};

class Plane : public Primitive {
public:
    virtual ~Plane();
    virtual void walk_gl() const;

};

class Cube : public Primitive {
public:
    virtual ~Cube();
    virtual void walk_gl() const;

};

#endif
