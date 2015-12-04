#include "Scene.h"
#include <iostream>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <glm/gtx/string_cast.hpp>

#include <GLES3/gl3.h>

using namespace glm;

typedef std::list<SceneNode*> ChildList;

int SceneNode::SCENE_NODE_COUNTER = 0;
int SceneNode::DL_INDEX = 3;

SceneNode::SceneNode(const std::string& name)
    : m_id(SCENE_NODE_COUNTER++)
    , m_name(name)
    , m_bPicked(false)
    , m_trans(1.0f)
    , m_children()
    , m_parent(0)
    , m_nRadius(0.0f)
{
}

SceneNode::~SceneNode()
{
    if (m_children.size() > 0) {
        // I can't remember if this is legit
        ChildList::iterator it;
        for (it = m_children.begin(); it != m_children.end(); it++) {
            delete *it;
        }
    }

}

SceneNode* SceneNode::clone()
{
    SceneNode* pNode = new SceneNode(*this);
    pNode->remove_children();

    // recursively call on my children
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        pNode->add_child((*it)->clone());
    }

    return pNode;
}

void SceneNode::get_centre(vec3& p)
{
    vec4 v(p[0], p[1], p[2], 1.0f);
    v = m_trans * v;

    p[0] = v[0];
    p[1] = v[1];
    p[2] = v[2];

    if (m_parent) {
        m_parent->get_centre(p);
    }
}

void SceneNode::render_shadow_volume(vec3 cube[][4],
                                     vec3& centre,
                                     vec3& light)
{
    //int i;
    //float v[4][3];

}


float SceneNode::get_radius()
{
    return m_nRadius;
}

// SPHERE STUFF
#define SPHERE_RADIUS 1.0f
#define SPHERE_SLICES 20
#define SPHERE_STACKS 20

static void makeSphere(int slices, int stacks, std::function<void(const vec3& position, const vec3& normal, const vec2& texcoord)> vertexToGeo)
{
    for (float t = 0; t < stacks; t++) {
        float theta1 = (t / stacks) * pi<float>() - half_pi<float>();
        float theta2 = ((t + 1) / stacks) * pi<float>() - half_pi<float>();

        for (float p = 0 ; p < slices ; p++) {
            float phi1 = (p / slices) * 2 * pi<float>() ;
            float phi2 = ((p + 1) / slices) * 2 * pi<float>() ;

            auto vertex1 = vec3(cos(theta1) * cos(phi1), cos(theta1) * sin(phi1), sin(theta1));
            auto tex1 = vec2(t / stacks, p / slices);

            auto vertex2 = vec3(cos(theta1) * cos(phi2), cos(theta1) * sin(phi2), sin(theta1));
            auto tex2 = vec2(t / stacks, (p + 1) / slices);

            auto vertex3 = vec3(cos(theta2) * cos(phi2), cos(theta2) * sin(phi2), sin(theta2));
            auto tex3 = vec2((t + 1) / stacks, p / slices);

            auto vertex4 = vec3(cos(theta2) * cos(phi1), cos(theta2) * sin(phi1), sin(theta2));
            auto tex4 = vec2((t + 1) / stacks, (p + 1) / slices);

            // facing out
            if (t == 0) {
                vertexToGeo(vertex1, vertex1, tex1);
                vertexToGeo(vertex3, vertex3, tex3);
                vertexToGeo(vertex4, vertex4, tex4);
            } else if (t + 1 == stacks) {
                vertexToGeo(vertex3, vertex3, tex3);
                vertexToGeo(vertex1, vertex1, tex1);
                vertexToGeo(vertex2, vertex2, tex4);
            } else {
                vertexToGeo(vertex1, vertex1, tex1);
                vertexToGeo(vertex2, vertex2, tex2);
                vertexToGeo(vertex4, vertex4, tex4);
                vertexToGeo(vertex2, vertex2, tex2);
                vertexToGeo(vertex3, vertex3, tex3);
                vertexToGeo(vertex4, vertex4, tex4);
            }
        }
    }
}

void SceneNode::setupDL()
{
    std::vector<float> geometry;
    geometry.reserve(10000);

    auto vertexToGeo = [&geometry](const vec3 & position, const vec3 & normal, const vec2 & texcoord) {
        geometry.push_back(position[0]);
        geometry.push_back(position[1]);
        geometry.push_back(position[2]);
        geometry.push_back(normal[0]);
        geometry.push_back(normal[1]);
        geometry.push_back(normal[2]);
        geometry.push_back(texcoord[0]);
        geometry.push_back(texcoord[1]);
    };
    makeSphere(SPHERE_SLICES, SPHERE_STACKS, vertexToGeo);

    std::cout << "Finished sphere geometry. Vertex Count " << geometry.size() / 8 << std::endl;

    // Make plane
    vec3 vNorm(0.0f, 1.0f, 0.0f);
    vertexToGeo(vec3(-1, 0, -1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(-1, 0, 1), vNorm, vec2(0, 1));
    vertexToGeo(vec3(1, 0, 1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(-1, 0, -1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(1, 0, 1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(1, 0, -1), vNorm, vec2(1, 0));

    std::cout << "Finished plane geometry. Vertex Count " << geometry.size() / 8 << std::endl;

    // Front
    vNorm = vec3(0, 0, 1);
    vertexToGeo(vec3(-1, -1, 1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(1, -1, 1), vNorm, vec2(0, 1));
    vertexToGeo(vec3(1, 1, 1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(-1, -1, 1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(1, 1, 1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(-1, 1, 1), vNorm, vec2(1, 0));

    // Back
    vNorm = vec3(0, 0, -1);
    vertexToGeo(vec3(-1, -1, -1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(-1, 1, -1), vNorm, vec2(1, 0));
    vertexToGeo(vec3(1, 1, -1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(-1, -1, -1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(1, 1, -1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(1, -1, -1), vNorm, vec2(0, 1));

    // Left
    vNorm = vec3(-1, 0, 0);
    vertexToGeo(vec3(-1, -1, 1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(-1, 1, 1), vNorm, vec2(1, 0));
    vertexToGeo(vec3(-1, 1, -1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(-1, -1, 1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(-1, 1, -1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(-1, -1, -1), vNorm, vec2(0, 1));

    // Right
    vNorm = vec3(1, 0, 0);
    vertexToGeo(vec3(1, -1, 1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(1, -1, -1), vNorm, vec2(0, 1));
    vertexToGeo(vec3(1, 1, -1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(1, -1, 1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(1, 1, -1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(1, 1, 1), vNorm, vec2(1, 0));

    // Bottom
    vNorm = vec3(0, -1, 0);
    vertexToGeo(vec3(-1, -1, -1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(1, -1, -1), vNorm, vec2(0, 1));
    vertexToGeo(vec3(1, -1, 1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(-1, -1, -1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(1, -1, 1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(-1, -1, 1), vNorm, vec2(1, 0));

    // Top
    vNorm = vec3(0, 1, 0);
    vertexToGeo(vec3(-1, 1, -1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(-1, 1, 1), vNorm, vec2(1, 0));
    vertexToGeo(vec3(1, 1, 1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(-1, 1, -1), vNorm, vec2(0, 0));
    vertexToGeo(vec3(1, 1, 1), vNorm, vec2(1, 1));
    vertexToGeo(vec3(1, 1, -1), vNorm, vec2(0, 1));

    std::cout << "Finished cube geometry. Vertex Count " << geometry.size() / 8 << std::endl;

    // Stick the geometry in a vbo.
    unsigned vbo = 0;
    glGenBuffers(1, &vbo);

    std::cout << "Generated vbo " << vbo << std::endl;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, geometry.size() * sizeof(float), geometry.data(), GL_STATIC_DRAW);


}

void SceneNode::tick()
{
    // recursively call on my children
    if (!m_children.size())
        return;

    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        (*it)->tick();
    }
}

void SceneNode::colour(const Colour& c)
{
    // recursively call on my children
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        (*it)->colour(c);
    }
}

void SceneNode::set_shadow(bool b)
{
    // recursively call on my children
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        (*it)->set_shadow(b);
    }
}

void SceneNode::walk_gl2(const mat4& mat) const
{
    // Apply my transformation
    bool pushAndMult = m_trans != glm::mat4();

    mat4 next(mat);
    if (pushAndMult) {
        next = next * m_trans;
        glUniformMatrix4fv(1, 1, GL_FALSE, value_ptr(next));
    }

    draw_gl();

    for (auto& child : m_children)
        child->walk_gl2(next);

    if (pushAndMult) {
        glUniformMatrix4fv(1, 1, GL_FALSE, value_ptr(mat));
    }
}

SceneNode* SceneNode::find(const std::string& aName)
{
    SceneNode* node = 0;

    if (aName == m_name) {
        return this;
    }

    // Default assumption - walk_gl on my children
    ChildList::const_iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        node = (*it)->find(aName);

        if (node) {
            return node;
        }
    }

    return node;
}

void SceneNode::find_all(std::vector<SceneNode*>& v, const std::string& name)
{

    if (m_name == name) { // add me to vector
        v.push_back(this);
    }

    // walk the children
    ChildList::const_iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        (*it)->find_all(v, name);
    }

}

void SceneNode::draw_gl() const
{

}

int SceneNode::get_id() const
{
    return m_id;
}

SceneNode* SceneNode::get_parent() const
{
    return m_parent;
}

const std::string& SceneNode::get_name() const
{
    return m_name;
}

void SceneNode::toggle_picked()
{

    m_bPicked = !m_bPicked;
    // std::cout << "I am " << m_name << ". I am picked? " << m_bPicked << std::endl;

    // Set my children to be picked
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        if (!(*it)->is_joint()) {
            (*it)->set_picked(m_bPicked);
        }
    }

}

void SceneNode::get_picked_joints(std::list< SceneNode* >* pickedJoints)
{

    if (is_joint() && m_bPicked) {
        pickedJoints->push_back(this);
    }
    // Set my children to be picked
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        if ((*it)->is_joint()) {
            (*it)->get_picked_joints(pickedJoints);
        }
    }
}


void SceneNode::set_picked(bool picked)
{
    m_bPicked = picked;
}

// Toggle the picked status of this node
bool SceneNode::pick(int id)
{
    // I AM THIS ID
    if (m_id == id) {
        //std::cout << "Id " << id << " found. I am the " << m_name << ". Who is my parent? " << std::endl;
        // I am a jointnode
        if (is_joint()) {
            //std::cout << "This thing is a joint and is getting toggled." << std::endl;
            toggle_picked();
            return true;
        }

        // Parent is a jointnode ?
        // Parent is not a jointnode ?
        SceneNode* node = m_parent;
        while (node != 0) {
            if (node->is_joint()) {
                //std::cout << " -> Parent found! Parent is " << node->get_name() << std::endl;
                node->toggle_picked();
                return true;
            }
            node = node->get_parent();
        }

        return false;
    }

    // Not my id, check my children
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        if ((*it)->pick(id)) {
            return true;
        }
    }

    // Should not get here
    // std::cerr << "ID " << id << " not found !?!?!" << std::endl;
    return false;
}

void SceneNode::rotatePicked(int degrees)
{

    if (is_joint() && m_bPicked) {
        rotate('x', degrees);
    }

    // Now check children
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        (*it)->rotatePicked(degrees);
    }

}

void SceneNode::rotateHead(int degrees)
{

    if (is_joint() && m_bPicked && m_name == "head") {
        rotate('y', degrees);
        return;
    }

    // Now check children
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        (*it)->rotateHead(degrees);
    }

}

void SceneNode::resetOrientation()
{

    m_trans = glm::mat4(1.0f);

}

void SceneNode::rotate(char axis, float degs)
{
    // FIXME: THIS FUNCTION IS BROKEN.
    // It should be identical to the code here, but it isn't.
    // Since everything depends on it, I haven't changed it yet.
    //
    // vec3 v(axis == 'x' ? 1.0f : 0.0f, axis == 'y' ? 1.0f : 0.0f, axis == 'z' ? 1.0f : 0.0f);
    // m_trans = glm::rotate(m_trans, radians(degs), v);
    int topRow = 0;
    int bottomRow = 0;
    int leftCol = 0;
    int rightCol = 0;

    // Obtain the four locations for the trig functions
    switch (axis) {
    case 'x':
        topRow = 1;
        bottomRow = 2;
        leftCol = 1;
        rightCol = 2;
        break;
    case 'y':
        topRow = 0;
        bottomRow = 2;
        leftCol = 0;
        rightCol = 2;
        break;
    case 'z':
        topRow = 0;
        bottomRow = 1;
        leftCol = 0;
        rightCol = 1;
        break;
    default:
        std::cerr << "Bad params!" << std::endl;

    }

    mat4 rotation;
    float rads = radians(degs);
    rotation[topRow][leftCol] = cos(rads);
    rotation[bottomRow][leftCol] = sin(rads);
    rotation[topRow][rightCol] = -sin(rads);
    rotation[bottomRow][rightCol] = cos(rads);
    m_trans = m_trans * rotation;
}

void SceneNode::scale(const glm::vec3& amount)
{
    vec3 v(amount[0], amount[1], amount[2]);
    m_trans = glm::scale(m_trans, v);
}

void SceneNode::translate(const glm::vec3& amount)
{
    vec3 v(amount[0], amount[1], amount[2]);
    m_trans = glm::translate(m_trans, v);
}

bool SceneNode::is_joint() const
{
    return false;
}

JointNode::JointNode(const std::string& name)
    : SceneNode(name)
    , m_axis('x')
    , m_nAngle(0)
    , m_pDestFrame(0)
    , m_KeyFrames()
    , m_joint_x()
    , m_joint_y()
{
}

JointNode::~JointNode()
{
}

SceneNode* JointNode::clone()
{

    SceneNode* pNode = new JointNode(*this);
    pNode->remove_children();

    // recursively call on my children
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        pNode->add_child((*it)->clone());
    }

    return pNode;
}

void JointNode::draw_gl() const
{
    //if (picking) {
    // return;
    //}


}

bool JointNode::is_joint() const
{
    return true;
}

void JointNode::set_joint_x(float min, float init, float max)
{
    m_joint_x.min = min;
    m_joint_x.init = init;
    m_joint_x.max = max;
}

void JointNode::set_joint_y(float min, float init, float max)
{
    m_joint_y.min = min;
    m_joint_y.init = init;
    m_joint_y.max = max;
}

void JointNode::add_frame(AnnimationFrame* pFrame)
{
    m_KeyFrames.push_back(pFrame);
}

void JointNode::freeze()
{
    FrameList::iterator it;
    for (it = m_KeyFrames.begin(); it != m_KeyFrames.end(); it++) {
        (*it)->m_nAngle = m_nAngle;
        (*it)->m_nRemainingFrames = 0;
        (*it)->m_bLoopBack = false;
    }

    if (m_pDestFrame) {
        m_pDestFrame->m_bLoopBack = false;
        m_pDestFrame->m_nAngle = m_nAngle;
        m_pDestFrame->m_nRemainingFrames = 0;
    }

    m_KeyFrames.clear();
}

void JointNode::tick()
{

    // Call parent to call children
    SceneNode::tick();

    while (true) {
        // If no frame, but we have frames get one
        if (!m_pDestFrame && m_KeyFrames.size() > 0) {
            m_pDestFrame = m_KeyFrames.front();
            m_KeyFrames.pop_front();
        }
        // If no frame, quit
        else if (!m_pDestFrame) {
            return;
        }

        // Make sure this frame has frames remaining.
        if (m_pDestFrame->m_nRemainingFrames == 0) {
            delete m_pDestFrame;
            m_pDestFrame = 0;
        } else {
            break;
        }
    }

    if (m_pDestFrame->m_nRemainingFrames > 0) {
        // Prepare our next angle
        if ((m_pDestFrame->m_nAngle - m_nAngle) > 185) {
            m_pDestFrame->m_nAngle -= 360;
        } else if ((m_pDestFrame->m_nAngle - m_nAngle) < -185) {
            m_pDestFrame->m_nAngle += 360;
        }

        float nDiffStep = (m_pDestFrame->m_nAngle - m_nAngle)
                          / (float)m_pDestFrame->m_nRemainingFrames;

        rotate(m_axis, nDiffStep);

        // Add this to our current known angle;
        m_nAngle += nDiffStep;

        // Avoid overwrapping
        if (m_nAngle < 0) {
            m_nAngle += 360;
        } else if (m_nAngle > 360) {
            m_nAngle -= 360;
        }

        m_pDestFrame->m_nRemainingFrames--;

        if (m_pDestFrame->m_nRemainingFrames == 0) {

            // Either requeue or delete
            if (m_pDestFrame->m_bLoopBack) {
                m_pDestFrame->m_nRemainingFrames = m_pDestFrame->m_nFrames;
                m_KeyFrames.push_back(m_pDestFrame);
            } else {
                delete m_pDestFrame;
            }

            m_pDestFrame = 0;
        }

    }

}

const float& JointNode::get_angle()
{
    return m_nAngle;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
    : SceneNode(name)
    , m_material(0)
    , m_map(0)
    , m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
    /*
    if (m_material) {
      delete m_material;
      m_material = 0;
    }
    if (m_primitive) {
      delete m_primitive;
      m_primitive = 0;
    }*/
}

SceneNode* GeometryNode::clone()
{
    GeometryNode* pNode = new GeometryNode(*this);
    pNode->remove_children();
    pNode->set_material(m_material);
    pNode->set_primitive(m_primitive);

    // recursively call on my children
    ChildList::iterator it;
    for (it = m_children.begin(); it != m_children.end(); it++) {
        pNode->add_child((*it)->clone());
    }

    return pNode;
}

void GeometryNode::draw_gl() const
{

    // Blend if we have a bump map
    if (false)  {
        // Choose fifty fifty
        //glBlendFunc(GL_ONE, GL_ZERO);

        // Set my bumpmap
        m_map->apply_gl();

        // Choose fifty fifty
        //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

        // Draw my primitive
        m_primitive->walk_gl();

    }

    // Default blending
    // glBlendFunc(GL_ONE, GL_ZERO);

    // Set my texture
    m_material->apply_gl();

    // Draw my primitive
    m_primitive->walk_gl();
}

void GeometryNode::set_shadow(bool b)
{
    m_primitive->set_shadow(b);

    SceneNode::set_shadow(b);
}

void GeometryNode::colour(const Colour& d)
{

    ((PhongMaterial*)m_material)->m_kd = d;

    SceneNode::colour(d);
}
