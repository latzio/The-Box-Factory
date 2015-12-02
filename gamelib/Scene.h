#ifndef SCENE_HPP
#define SCENE_HPP

#include "Algebra.h"
#include "Primitive.h"
#include "Material.h"
#include "Macro.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <list>
#include <vector>

class AnnimationFrame {
public:
    AnnimationFrame() : m_nAngle(0), m_nFrames(0),
        m_nRemainingFrames(0), m_bLoopBack(false) {}

    double m_nAngle;
    int m_nFrames;
    int m_nRemainingFrames;
    bool m_bLoopBack;
};

class SceneNode {
public:
    SceneNode(const std::string& name);
    virtual ~SceneNode();

    SceneNode(const SceneNode&) = default;
    SceneNode& operator=(const SceneNode&) = default;

    // Return a complete copy of this scenenode
    virtual SceneNode* clone();

    virtual void tick();
    virtual void walk_gl() const;
    virtual void walk_gl2(const glm::mat4x4&) const;
    virtual void draw_gl() const;

    const Matrix4x4& get_transform() const
    {
        return m_trans;
    }
    const Matrix4x4& get_inverse() const
    {
        return m_invtrans;
    }

    void set_radius(double r)
    {
        m_nRadius = r;
    }
    double get_radius();

    void get_centre(Point3D& p);

    //const virtual Matrix4x4& get_

    void set_transform(const Matrix4x4& m)
    {
        m_trans = m;
        m_invtrans = m.invert();
    }

    void set_transform(const Matrix4x4& m, const Matrix4x4& i)
    {
        m_trans = m;
        m_invtrans = i;
    }

    void set_parent(SceneNode* parent)
    {
        m_parent = parent;
    }

    void add_child(SceneNode* child)
    {
        m_children.push_back(child);
        child->set_parent(this);
    }

    void remove_child(SceneNode* child)
    {
        m_children.remove(child);
    }

    void remove_children()
    {
        m_children.clear();
    }

    // Callbacks to be implemented.
    // These will be called from Lua.
    void rotate(char axis, double angle);
    void scale(const Vector3D& amount);
    void translate(const Vector3D& amount);

    void rotatePicked(int degrees);
    void rotateHead(int degrees);

    void resetOrientation();

    // These are needed for undo
    std::vector< int >* getPicked(std::vector< int >* pPicked);
    void applyAction(Matrix4x4* pTransform);

    // Returns true if and only if this node is a JointNode
    virtual bool is_joint() const;

    // Called to recursively pick a node with this id
    bool pick(int id);
    void toggle_picked();
    void set_picked(bool picked);

    void get_picked_joints(std::list< SceneNode* >* pickedJoints);

    int get_id() const;
    const std::string& get_name() const;
    SceneNode* get_parent() const;
    SceneNode* find(const std::string& aName);
    void find_all(std::vector<SceneNode*>& v, const std::string& name);

    void undo();
    void redo();

    // setup display list
    static void setupDL();

    // render shadow volume
    static void render_shadow_volume(Point3D cube[][4], Point3D& centre, Point3D& light);

    static int DL_INDEX;
    enum DL_INDEX { DL_SPHERE, DL_PLANE, DL_CUBE };

    // Activate shadows for me and children
    virtual void set_shadow(bool b);

    virtual void colour(const Colour& c);

protected:

    static int SCENE_NODE_COUNTER;

    // Useful for picking
    int m_id;
    std::string m_name;
    bool m_bPicked;

    // Transformations
    Matrix4x4 m_trans;
    mutable Matrix4x4 m_transTranspose;
    Matrix4x4 m_invtrans;

    // Hierarchy
    typedef std::list<SceneNode*> ChildList;
    ChildList m_children;

    SceneNode* m_parent;

    // Collisions
    double m_nRadius;

    // Optimization
    mutable bool m_dirty;

};

class JointNode : public SceneNode {
public:
    JointNode(const std::string& name);
    virtual ~JointNode();

    CopyDefault(JointNode)

    virtual void tick();
    virtual SceneNode* clone();

    virtual void draw_gl() const;
    virtual void walk_gl() const;

    virtual bool is_joint() const;

    void set_axis(char a)
    {
        m_axis = a;
    }
    void set_joint_x(double min, double init, double max);
    void set_joint_y(double min, double init, double max);

    void freeze();
    void add_frame(AnnimationFrame* pFrame);
    const double& get_angle();

    struct JointRange {
        double min, init, max;
    };


protected:

    char m_axis;

    typedef std::list<AnnimationFrame*> FrameList;
    double m_nAngle;
    AnnimationFrame* m_pDestFrame;

    FrameList m_KeyFrames;

    JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
public:
    GeometryNode(const std::string& name,
                 Primitive* primitive);
    virtual ~GeometryNode();

    CopyDefault(GeometryNode)

    virtual void draw_gl() const;
    // Return a complete copy of this scenenode
    virtual SceneNode* clone();

    const Material* get_material() const;
    Material* get_material();

    void set_material(Material* material)
    {
        m_material = material;
    }

    const Material* get_map() const;
    Material* get_map();

    void set_map(Material* map)
    {
        m_map = map;
    }

    void set_primitive(Primitive* primitive)
    {
        m_primitive = primitive;
    }

    // Activate shadows for me and children
    virtual void set_shadow(bool b);

    virtual void colour(const Colour& c);

protected:
    Material* m_material;
    Material* m_map;
    Primitive* m_primitive;
};

#endif
