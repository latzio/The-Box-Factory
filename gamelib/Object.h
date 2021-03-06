#ifndef SMASHTVOBJECT_H_
#define SMASHTVOBJECT_H_

#include "Scene.h"
#include "SceneLua.h"
#include "AI.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

// This is the base class for all moving objects
// in the Smashtv game, including players, enemies, and
// flying bullets, Movable children will possibly support animation
// as well as collision detection to make it playable

class Graphics;
class MoveableSubscriber;
class NPC;

class Moveable {

public:
    Moveable();
    Moveable(SceneNode* element, MoveableSubscriber* subscriber);
    virtual ~Moveable();
    MoveCtor(Moveable)

    //void set_position(const Matrix4x4&);
    void set_position(const glm::vec3&);
    // Step through my animation/movement
    // (Called by gamelogic)
    virtual void tick();

    // draw me to gl buffers
    // (Called by renderer)
    virtual void walk_gl2(Graphics&);

    // see if this moveable hits this object
    virtual Moveable* IsHit(const glm::vec3& p, float r);

    virtual bool is_dead()
    {
        return m_bDead;
    }
    virtual void set_dead(bool b)
    {
        m_bDead = b;
    }

    // Where is this unit
    virtual void getCentre(glm::vec3& p);

    static int WATERMARK_LOW;
    static int WATERMARK_HIGH;

    // Needed for RTTI
    enum Type { TYPE_MOVEABLE, TYPE_BULLET, TYPE_PARTICLE, TYPE_LEVEL, TYPE_SHIELD,
                TYPE_SPARK, TYPE_OBSTACLE, TYPE_NPC, TYPE_NPC_PISTOL, TYPE_PC
              };
    virtual Type getType()
    {
        return TYPE_MOVEABLE;
    }


    SceneNode* getRoot()
    {
        return m_pElement;
    }

protected:

    SceneNode* m_pElement;
    MoveableSubscriber* m_pSubscriber;

    bool m_bDead;

};

class Shield : public Moveable {
public:
    Shield(SceneNode* element, MoveableSubscriber* subscriber);
    virtual ~Shield();

    // Step through my animation/movement
    // (Called by gamelogic)
    virtual void tick();

    virtual Moveable* IsHit(const glm::vec3& p, float r);

    void reset();

    void setTTL(int ttl);
    int getTTL()
    {
        return m_nTTL;
    }

    void setMajor(bool b)
    {
        m_bMajor = b;
    }
    bool isMajor()
    {
        return m_bMajor;
    }

    virtual Type getType()
    {
        return TYPE_SHIELD;
    }

private:
    bool m_bMajor;
    int m_nTTLMax;
    int m_nTTL;


};

class Particle : public Moveable {
public:
    Particle(SceneNode* element, MoveableSubscriber* subscriber);
    virtual ~Particle();

    virtual void tick();

    void set_velocity(const glm::vec3& v)
    {
        m_velocity = v;
    }
    virtual void reset();

    virtual Type getType()
    {
        return TYPE_PARTICLE;
    }
protected:

    virtual void addGravity();

    float m_nAngularVelocity;
    glm::vec3 m_velocity;

    int m_nTTL;
};

class Spark : public Particle {
public:
    Spark(SceneNode* element, MoveableSubscriber* subscriber);
    virtual ~Spark();

    virtual void tick();
    virtual void reset();

    virtual Type getType()
    {
        return TYPE_SPARK;
    }
protected:
    virtual void addGravity();
};

class Level : public Moveable {
public:
    Level(SceneNode* element, MoveableSubscriber* subscriber);
    virtual ~Level();
    MoveCtor(Level)

    virtual Type getType()
    {
        return TYPE_LEVEL;
    }

    // Animate the doors
    virtual void tick();

    enum Door { DOOR_FRONT, DOOR_RIGHT, DOOR_BACK, DOOR_LEFT };
    virtual void closeDoor(Door d);
    virtual void openDoor(Door d);

protected:

    static AnnimationFrame* createDoorFrame(int nAngle);

    typedef std::vector< SceneNode* > ObjList;
    ObjList m_Objs;

    JointNode* m_pDoor[4];
    bool m_bDoorOpen[4];


};

class Obstacle : public Moveable {
public:
    Obstacle(const glm::vec3& centre, float radius);
    virtual ~Obstacle();

    // see if this moveable hits this object
    virtual void getCentre(glm::vec3& p)
    {
        p = m_centre;
    }
    virtual Moveable* IsHit(const glm::vec3& p, float r);
    virtual Type getType()
    {
        return TYPE_OBSTACLE;
    }
protected:
    glm::vec3 m_centre;
    float m_nRadius;
};

class Bullet : public Moveable {

public:
    Bullet(SceneNode* element, MoveableSubscriber* subscriber, NPC* source);
    virtual ~Bullet();
    MoveCtor(Bullet)

    // Use this with the constructor to create bullets from
    // copies of the static bullet
    static Bullet* CreateBullet(MoveableSubscriber* subscriber,
                                Moveable* source);

    virtual void tick();

    void set_joint();
    void set_direction(float dDegrees);
    void set_velocity(float v);

    virtual Type getType()
    {
        return TYPE_BULLET;
    }

protected:
    static Bullet* DEFAULT_BULLET;

    JointNode* m_pDirectionNode;
    SceneNode* m_pTrajectoryNode;
    Type m_eSource;

    //float m_direction;

    float m_nVelocity;
    float m_nWobble;
    int m_nTTL;
    int m_nPower;       // Damage done by this bullet


};

class NPC : public Moveable, public AISubscriber {

public:
    NPC();
    NPC(SceneNode* element, MoveableSubscriber* subscriber);
    virtual ~NPC();
    MoveCtor(NPC)

    virtual void tick();

    virtual bool isNPC()
    {
        return true;
    }

    // These four calls toss various rotations in to keyframe animation
    virtual void updateMovingAnimation();
    virtual void updateMovingDirection(bool bCascade = true);
    virtual void updateAimingAnimation();
    virtual void updateAimingDirection(bool bCascade = true);

    // NPCs have health
    virtual void setHealth(int nHealth)
    {
        m_nHealth = nHealth;
    }
    virtual bool damage(int nHealth); // return true if health < 0 after damage

    enum Direction { DOWN = 0, RIGHT = 1, UP = 2, LEFT = 3 };
    enum OutputDir { NO_MOVEMENT = -1, D = 0, DR = 1, R = 2, UR = 3, U = 4, UL = 5, L = 6, DL = 7 };
    OutputDir getOutputDirection();
    OutputDir getTargetDirection();

    // Enum of significant joints
    enum Joint {     JOINT_CENTRE,
                     JOINT_UPPER_CENTRE,
                     JOINT_RIGHT_SHOULDER,
                     JOINT_RIGHT_ELBOW,
                     JOINT_RIGHT_WRIST,
                     JOINT_RIGHT_HIP,
                     JOINT_RIGHT_KNEE,
                     JOINT_LEFT_SHOULDER,
                     JOINT_LEFT_ELBOW,
                     JOINT_LEFT_WRIST,
                     JOINT_LEFT_HIP,
                     JOINT_LEFT_KNEE
               };

    // Control player input
    void setMoving(Direction d, bool b);
    void setAiming(Direction d, bool b);

    // Is player moving or shooting (at all)
    bool isMoving();
    bool isAiming();

    // Give me the point from which bullets come out of this Moveable
    const virtual glm::vec3 get_gun_nozzle() ;

    // see if this moveable hits this object
    virtual Moveable* IsHit(const glm::vec3& p, float r);

    // Move this unit, true if success, false if impeded
    virtual bool move(AI::Direction ePrimary, AI::Direction eSecondary);

    // Where is this unit
    virtual void getCentre(glm::vec3& p);

    // Revert to general purpose AI
    virtual void revertToAI();

    // Is this unit alive?
    virtual bool isActive()
    {
        return !m_bDead;
    };

    virtual Type getType()
    {
        return TYPE_NPC;
    }

    virtual void setCooldown(int cooldown)
    {
        m_nCooldownReset = cooldown;
    }
    virtual void setThrottle(float throttle)
    {
        m_nThrottle = throttle;
    }

protected:

    virtual Moveable* getCollidingMoveable();
    virtual void doCollisionAction(Moveable* m, const glm::vec3& v);
    virtual void doUniqueAction() { };

    // THE NEW WAY
    bool m_eMoving[4];
    bool m_eAiming[4];

    int m_nCooldown;

    float m_nThrottle; // Multiplied by the unit step each movement

    // Pointers to important nodes, must be size of Joint enum
    JointNode* m_pJoints[12];

    int m_nHealth;

    int m_nCooldownReset;


};

class PistolNPC : public NPC {
public:

    PistolNPC(SceneNode* element, MoveableSubscriber* subscriber);
    virtual ~PistolNPC();

    virtual Type getType()
    {
        return TYPE_NPC_PISTOL;
    }

protected:
    virtual void doUniqueAction();

    int m_nCooldown;
};

class PC : public NPC {

public:
    PC();
    PC(SceneNode* element, MoveableSubscriber* subscriber);
    virtual ~PC();
    MoveCtor(PC)

    virtual void tick();

    virtual bool isNPC()
    {
        return false;
    }
    const virtual glm::vec3 get_gun_nozzle();

    // see if this moveable hits this object
    virtual Moveable* IsHit(const glm::vec3& p, float r);
    virtual Type getType()
    {
        return TYPE_PC;
    }

    virtual void setHumanControlled(bool b)
    {
        m_bHumanControlled = b;
    }
    virtual bool isHumanControlled()
    {
        return m_bHumanControlled;
    }

    virtual void die();

protected:
    virtual Moveable* getCollidingMoveable();
    virtual void doCollisionAction(Moveable* m, const glm::vec3& v);

private:

    bool m_bRightSideShot;
    bool m_bHumanControlled;

    SceneNode* m_pNozzle[2];
    Shield* m_pShield;

};

class MoveableSubscriber {

public:
    virtual ~MoveableSubscriber() { }

    // These are required for shooting
    virtual void CreateBullet(const glm::vec3& origin, float dDegrees, NPC* source) = 0;
    virtual void DeleteBullet(Bullet* pBullet) = 0;
    virtual void CreateAI(NPC* npc) = 0;

    enum ShieldType { MAJOR, MINOR };
    virtual Shield* RequestShield(ShieldType s) = 0;

    // filling the "grid"
    // virtual void RegisterMoveable(Moveable* moveable, int radius) = 0;

    // detect collision
    virtual Moveable* DetectCollision(const glm::vec3& p, float r, Moveable* pExcluded = NULL) = 0;
    virtual void DamageEnemy(NPC* pNPC, int nDamage, const glm::vec3& p3d) = 0;
    virtual void CreateObstacle(Moveable* pObstacle) = 0;

    enum ParticleSize { SIZE_LITTLE = 0, SIZE_MEDIUM = 1, SIZE_BIGGER = 2, SIZE_SPARKS = 3 };
    virtual void CreateParticles(ParticleSize eSize, int nQuantity, const glm::vec3& v) = 0;

    enum SFX { SFX_BULLET, SFX_BLAST, SFX_BOMB, SFX_CAROM, SFX_HUMAN_DEATH, SFX_ROBOT_HIT, SFX_ROBOT_DEATH };
    virtual void PlaySFX(SFX id) = 0;
};

#endif
