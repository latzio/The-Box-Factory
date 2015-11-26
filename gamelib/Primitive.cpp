#include "Primitive.h"
#include "Scene.h"
#include "Textures.h"

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
    // draw the display list
    glCallList(SceneNode::DL_INDEX + SceneNode::DL_SPHERE);
}

Plane::~Plane()
{
}

void Plane::walk_gl() const
{
    glCallList(SceneNode::DL_INDEX + SceneNode::DL_PLANE);
}

Cube::~Cube()
{
}

void Cube::walk_gl() const
{
    glCallList(SceneNode::DL_INDEX + SceneNode::DL_CUBE);

    if (m_bShadow) {
        std::cout << "Rendering shadow volume." << std::endl;

#define LBOUND -1
#define HBOUND 1

        Point3D cube[6][4];

        cube[0][0] = Point3D(LBOUND, LBOUND, HBOUND);
        cube[0][1] = Point3D(HBOUND, LBOUND, HBOUND);
        cube[0][2] = Point3D(HBOUND, HBOUND, HBOUND);
        cube[0][3] = Point3D(LBOUND, HBOUND, HBOUND);

        // Back
        cube[1][0] = Point3D(LBOUND, LBOUND, LBOUND);
        cube[1][1] = Point3D(LBOUND, HBOUND, LBOUND);
        cube[1][2] = Point3D(HBOUND, HBOUND, LBOUND);
        cube[1][3] = Point3D(HBOUND, LBOUND, LBOUND);

        // Left
        cube[2][0] = Point3D(LBOUND, LBOUND, HBOUND);
        cube[2][1] = Point3D(LBOUND, HBOUND, HBOUND);
        cube[2][2] = Point3D(LBOUND, HBOUND, LBOUND);
        cube[2][3] = Point3D(LBOUND, LBOUND, LBOUND);

        // Right
        cube[3][0] = Point3D(HBOUND, LBOUND, HBOUND);
        cube[3][1] = Point3D(HBOUND, LBOUND, LBOUND);
        cube[3][2] = Point3D(HBOUND, HBOUND, LBOUND);
        cube[3][3] = Point3D(HBOUND, HBOUND, HBOUND);

        // Bottom
        cube[4][0] = Point3D(LBOUND, LBOUND, LBOUND);
        cube[4][1] = Point3D(HBOUND, LBOUND, LBOUND);
        cube[4][2] = Point3D(HBOUND, LBOUND, HBOUND);
        cube[4][3] = Point3D(LBOUND, LBOUND, HBOUND);

        // Top
        cube[5][0] = Point3D(LBOUND, HBOUND, LBOUND);
        cube[5][1] = Point3D(LBOUND, HBOUND, HBOUND);
        cube[5][2] = Point3D(HBOUND, HBOUND, HBOUND);
        cube[5][3] = Point3D(HBOUND, HBOUND, LBOUND);

        Point3D light(0, 20, 0);
        Point3D centre;
        //get_centre(centre);
    }
}
