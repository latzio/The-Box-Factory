#include "Primitive.h"
#include "Scene.h"
#include "Textures.h"

#include <iostream>

using namespace glm;

Primitive::Primitive()
    : m_bShadow(0)
{ }

Primitive::~Primitive()
{
}


Sphere::~Sphere()
{
}

void Sphere::walk_gl() const
{
    glDrawArrays(GL_TRIANGLES, 0, 2280);
}

Plane::~Plane()
{
}

void Plane::walk_gl() const
{
    glDrawArrays(GL_TRIANGLES, 2280, 6);
}

Cube::~Cube()
{
}

void Cube::walk_gl() const
{
    glDrawArrays(GL_TRIANGLES, 2286, 36);

    if (m_bShadow) {
        std::cout << "Rendering shadow volume." << std::endl;

#define LBOUND -1
#define HBOUND 1

        vec3 cube[6][4];

        cube[0][0] = vec3(LBOUND, LBOUND, HBOUND);
        cube[0][1] = vec3(HBOUND, LBOUND, HBOUND);
        cube[0][2] = vec3(HBOUND, HBOUND, HBOUND);
        cube[0][3] = vec3(LBOUND, HBOUND, HBOUND);

        // Back
        cube[1][0] = vec3(LBOUND, LBOUND, LBOUND);
        cube[1][1] = vec3(LBOUND, HBOUND, LBOUND);
        cube[1][2] = vec3(HBOUND, HBOUND, LBOUND);
        cube[1][3] = vec3(HBOUND, LBOUND, LBOUND);

        // Left
        cube[2][0] = vec3(LBOUND, LBOUND, HBOUND);
        cube[2][1] = vec3(LBOUND, HBOUND, HBOUND);
        cube[2][2] = vec3(LBOUND, HBOUND, LBOUND);
        cube[2][3] = vec3(LBOUND, LBOUND, LBOUND);

        // Right
        cube[3][0] = vec3(HBOUND, LBOUND, HBOUND);
        cube[3][1] = vec3(HBOUND, LBOUND, LBOUND);
        cube[3][2] = vec3(HBOUND, HBOUND, LBOUND);
        cube[3][3] = vec3(HBOUND, HBOUND, HBOUND);

        // Bottom
        cube[4][0] = vec3(LBOUND, LBOUND, LBOUND);
        cube[4][1] = vec3(HBOUND, LBOUND, LBOUND);
        cube[4][2] = vec3(HBOUND, LBOUND, HBOUND);
        cube[4][3] = vec3(LBOUND, LBOUND, HBOUND);

        // Top
        cube[5][0] = vec3(LBOUND, HBOUND, LBOUND);
        cube[5][1] = vec3(LBOUND, HBOUND, HBOUND);
        cube[5][2] = vec3(HBOUND, HBOUND, HBOUND);
        cube[5][3] = vec3(HBOUND, HBOUND, LBOUND);

        vec3 light(0, 20, 0);
        vec3 centre;
        //get_centre(centre);
    }
}
