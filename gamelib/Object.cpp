#include "Object.h"
#include <algorithm>

#define PLAYER_COOLDOWN 5 // 20 // Shotgun, return to 4 for uzi
#define ENEMY_COOLDOWN 60 // one per second

#define BULLET_TTL 600 // Max ten seconds of life
#define PARTICLE_TTL 240
#define SPARK_TTL 15
#define JOG_FRAME_LENGTH 16

#define DOOR_FRAME_LENGTH 10
#define SHIELD_FRAME_LENGTH 15

int Moveable::WATERMARK_LOW  = (RAND_MAX / 10) * 1;
int Moveable::WATERMARK_HIGH = (RAND_MAX / 10) * 5;

Moveable::Moveable()
    : m_pElement(0)
    , m_pSubscriber(0)
    , m_bDead(true)
{
}

Moveable::Moveable(SceneNode* element, MoveableSubscriber* subscriber)
    : m_pElement(element)
    , m_pSubscriber(subscriber)
    , m_bDead(true)
{
}

Moveable::~Moveable()
{
    if (m_pElement) {
        delete m_pElement;
    }
}

void Moveable::getCentre(Point3D& p)
{
    m_pElement->get_centre(p);
}

Moveable* Moveable::IsHit(const Point3D& p, double r)
{
    if (p[0] > r) {
        return 0;
    }
    return (Moveable*)0;
}

void Moveable::tick()
{
}

void Moveable::walk_gl()
{

    m_pElement->walk_gl();
}

void Moveable::walk_gl2(const glm::mat4x4& mat)
{

    m_pElement->walk_gl2(mat);
}

Shield::Shield(SceneNode* element, MoveableSubscriber* subscriber)
    : Moveable(element, subscriber)
    , m_bMajor(false)
    , m_nTTLMax(0)
    , m_nTTL(0)
{

    JointNode* pCog = static_cast<JointNode*>(element->find("cog"));
    pCog->set_axis('y');
    pCog->freeze();

    AnnimationFrame* pFrame;

    pFrame = new AnnimationFrame();
    pFrame->m_nFrames = SHIELD_FRAME_LENGTH;
    pFrame->m_nRemainingFrames = SHIELD_FRAME_LENGTH;
    pFrame->m_bLoopBack = true;
    pFrame->m_nAngle = 120;
    pCog->add_frame(pFrame);

    pFrame = new AnnimationFrame();
    pFrame->m_nFrames = SHIELD_FRAME_LENGTH;
    pFrame->m_nRemainingFrames = SHIELD_FRAME_LENGTH;
    pFrame->m_bLoopBack = true;
    pFrame->m_nAngle = 240;
    pCog->add_frame(pFrame);

    pFrame = new AnnimationFrame();
    pFrame->m_nFrames = SHIELD_FRAME_LENGTH;
    pFrame->m_nRemainingFrames = SHIELD_FRAME_LENGTH;
    pFrame->m_bLoopBack = true;
    pFrame->m_nAngle = 0;
    pCog->add_frame(pFrame);

}

Shield::~Shield() { }

void Shield::tick()
{

    Point3D p;
    m_pElement->get_centre(p);

    Moveable* m = m_pSubscriber->DetectCollision(p, 5);

    // Check for collision
    if (m) {
        // Have we hit an enemy? If shot by player do damage
        if ((m->getType() == Moveable::TYPE_NPC ||
                m->getType() == Moveable::TYPE_NPC_PISTOL)) {

            Point3D pHit;
            dynamic_cast<NPC*>(m)->getCentre(pHit);

            m_pSubscriber->DamageEnemy((NPC*)(m), 10000, pHit);
        }
    }

    if (m_nTTL) {
        m_nTTL--;
    }
}

void Shield::setTTL(int ttl)
{
    m_nTTLMax = ttl;
    reset();
}

void Shield::reset()
{
    m_nTTL = m_nTTLMax;
}

Moveable* Shield::IsHit(const Point3D& p, double r)
{

    Point3D pCentre;
    m_pElement->get_centre(pCentre);


    double nRadius = r + 4; //std::max(r, m_pElement->get_radius());
    double nDifference = (p - pCentre).length();

    if (nRadius > nDifference) {
        return this;
    }

    return (Moveable*)0;
}


Particle::Particle(SceneNode* element, MoveableSubscriber* subscriber)
    : Moveable(element, subscriber)
    , m_nAngularVelocity(0)
    , m_velocity()
    , m_nTTL(PARTICLE_TTL)
{


}

Particle::~Particle() { }

void Particle::reset()
{

    double xRand = ((rand() - (RAND_MAX / 2)) / (double)RAND_MAX);
    double zRand = ((rand() - (RAND_MAX / 2)) / (double)RAND_MAX);

    m_velocity = Vector3D(xRand, 4, zRand);

    m_nTTL = PARTICLE_TTL;
}

void Particle::tick()
{
    if (!m_nTTL--) {
        set_dead(true);
        return;
    }

    // Gravity affects different things differently
    addGravity();

    m_pElement->translate(m_velocity);
}

void Particle::addGravity()
{

    Point3D p;
    m_pElement->get_centre(p);
    if (p[1] < 0) {
        m_velocity[0] = 0.5 * m_velocity[0];
        m_velocity[1] = -0.25 * m_velocity[1];
        m_velocity[2] = 0.5 * m_velocity[2];
    } else {
        m_velocity[1] -= 0.5;
    }

}

Spark::Spark(SceneNode* element, MoveableSubscriber* subscriber)
    : Particle(element, subscriber)
{
    double rRand = ((rand() - (RAND_MAX / 2)) / (double)RAND_MAX);
    m_nAngularVelocity = (rRand * 22.5);
}

Spark::~Spark() { }

void Spark::addGravity() { }

void Spark::reset()
{
    Particle::reset();

    double yRand = ((rand() - (RAND_MAX / 2)) / (double)RAND_MAX);
    m_velocity[1] = yRand * 3;
    m_nTTL = SPARK_TTL;
}

void Spark::tick()
{
    m_pElement->rotate('y', m_nAngularVelocity);
    m_pElement->rotate('z', m_nAngularVelocity);
    Particle::tick();
}

Level::Level(SceneNode* element, MoveableSubscriber* subscriber)
    : Moveable(element, subscriber)
    , m_Objs()
{
    m_pElement->find_all(m_Objs, "box");
    m_pElement->find_all(m_Objs, "box_small");
    Obstacle* o;

    for (ObjList::iterator it = m_Objs.begin(); it != m_Objs.end(); it++) {
        Point3D p;
        (*it)->get_centre(p);
        p[0] = 5 * p[0];
        p[1] = 5 * p[1];
        p[2] = 5 * p[2];
        if ((*it)->get_name() == "box") {
            (*it)->set_radius(10);
        } else if ((*it)->get_name() == "box_small") {
            (*it)->set_radius(5);
        }
        o = new Obstacle(p, (*it)->get_radius());
        m_pSubscriber->CreateObstacle(o);

    }

    //create outer walls - top and bottom
    for (int i = -85; i < 90; i += 20) {
        o = new Obstacle(Point3D(i, 5, -70), 10);
        m_pSubscriber->CreateObstacle(o);
        o = new Obstacle(Point3D(i, 5, 70), 10);
        m_pSubscriber->CreateObstacle(o);
    }
    //create outer walls - left and right
    for (int i = -65; i < 70; i += 20) {
        o = new Obstacle(Point3D(-90, 5, i), 10);
        m_pSubscriber->CreateObstacle(o);
        o = new Obstacle(Point3D(90, 5, i), 10);
        m_pSubscriber->CreateObstacle(o);
    }

    // Load the door references
    m_pDoor[DOOR_FRONT] = static_cast<JointNode*>(m_pElement->find("door_front"));
    m_pDoor[DOOR_RIGHT] = static_cast<JointNode*>(m_pElement->find("door_right"));
    m_pDoor[DOOR_BACK]  = static_cast<JointNode*>(m_pElement->find("door_back"));
    m_pDoor[DOOR_LEFT]  = static_cast<JointNode*>(m_pElement->find("door_left"));

    m_pDoor[DOOR_RIGHT]->set_axis('z');
    m_pDoor[DOOR_RIGHT]->rotate('y', 180);


    m_pDoor[DOOR_LEFT]->set_axis('z');

    std::fill(&m_bDoorOpen[0], &m_bDoorOpen[4], false);
}

Level::~Level() { }

AnnimationFrame* Level::createDoorFrame(int nAngle)
{
    AnnimationFrame* pFrame = new AnnimationFrame();
    pFrame->m_nFrames = DOOR_FRAME_LENGTH;
    pFrame->m_nRemainingFrames = DOOR_FRAME_LENGTH;
    pFrame->m_bLoopBack = false;
    pFrame->m_nAngle = nAngle;

    return pFrame;
}

void Level::closeDoor(Door d)
{
    if (m_bDoorOpen[(int)d]) {
        m_bDoorOpen[(int)d] = false;
        m_pDoor[(int)d]->freeze();
        m_pDoor[(int)d]->add_frame(createDoorFrame(0));
    }
}

void Level::openDoor(Door d)
{
    if (!m_bDoorOpen[(int)d]) {
        m_bDoorOpen[(int)d] = true;
        m_pDoor[(int)d]->freeze();
        m_pDoor[(int)d]->add_frame(createDoorFrame(-90));
    }
}

void Level::tick()
{
    m_pElement->tick();
}


Obstacle::Obstacle(const Point3D& centre, double radius)
    : m_centre(centre)
    , m_nRadius(radius)
{ }

Obstacle::~Obstacle() { }

Moveable* Obstacle::IsHit(const Point3D& p, double r)
{

    bool bHit = true;
    double nRadius = r + m_nRadius;

    // Check X
    if ((abs(p[0] - m_centre[0])) > nRadius ||
            (abs(p[1] - m_centre[1])) > nRadius ||
            (abs(p[2] - m_centre[2])) > nRadius) {
        bHit = false;
    }

    if (bHit) {
        return this;
    }

    return (Moveable*)0;
}
////////////////////////////////////////////////////////////////////////////////////

Bullet::Bullet(SceneNode* element, MoveableSubscriber* subscriber, NPC* source)
    : Moveable(element, subscriber)
    , m_pDirectionNode(0)
    , m_pTrajectoryNode(0)
    , m_eSource(source->getType())
    , m_nVelocity(1)
    , m_nWobble(0)
    , m_nTTL(BULLET_TTL)
    , m_nPower(35)
{
    m_nWobble = ((rand() / (double)RAND_MAX) - 0.5) * 0.5 * m_nVelocity;

}

Bullet::~Bullet()
{
}

void Bullet::tick()
{

    Point3D pNow;
    m_pTrajectoryNode->get_centre(pNow);

    m_pTrajectoryNode->translate(Vector3D(m_nWobble, 0, m_nVelocity));

    Point3D p;
    m_pTrajectoryNode->get_centre(p);

    Moveable* m = m_pSubscriber->DetectCollision(p, 0.1);

    // Set to true if this bullet is finished
    bool bDelete = false;

    // Check for collision
    if (m) {
        // Have we hit a wall? Show sparks and die
        if (m->getType() == Moveable::TYPE_OBSTACLE) {
            m_pSubscriber->CreateParticles(MoveableSubscriber::SIZE_SPARKS, 4, pNow - Point3D(0, 0, 0));
            bDelete = true;

            if (m_eSource == Moveable::TYPE_PC) {
                m_pSubscriber->PlaySFX(MoveableSubscriber::SFX_CAROM);
            }
        }
        // Have we hit an enemy? If shot by player do damage
        else if ((m->getType() == Moveable::TYPE_NPC ||
                  m->getType() == Moveable::TYPE_NPC_PISTOL)
                 && m_eSource == Moveable::TYPE_PC)  {

            m_pSubscriber->DamageEnemy((NPC*)(m), m_nPower, p);
            bDelete = true;
        }
        // Have we hit a player? If shot by an enemy kill player
        else if (m->getType() == Moveable::TYPE_PC
                 && m_eSource == Moveable::TYPE_NPC_PISTOL)   {
            m_pSubscriber->DamageEnemy((NPC*)m, 1000, p);
            bDelete = true;
        } // a shield?
        else if (m->getType() == Moveable::TYPE_SHIELD && m_eSource == Moveable::TYPE_NPC_PISTOL) {
            bDelete = true;
        }

    }

    // Bullets aren't really animated, but just incase
    // m_pElement->tick();

    // Delete after a time
    if (bDelete || m_nTTL-- < 0) {
        m_pSubscriber->DeleteBullet(this);
    }


}

void Bullet::walk_gl()
{
    Moveable::walk_gl();
}

void Bullet::set_joint()
{
    m_pDirectionNode = static_cast<JointNode*>(m_pElement->find("cog")) ;
    m_pTrajectoryNode = static_cast<JointNode*>(m_pElement->find("travel")) ;
}

void Bullet::set_direction(double dDegrees)
{

    m_nTTL = BULLET_TTL;
    if (m_pDirectionNode) {
        Matrix4x4 m;
        m_pTrajectoryNode->set_transform(m);
        m_pDirectionNode->set_transform(m);
        m_pDirectionNode->rotate('y', dDegrees);
    }

}

void Bullet::set_velocity(double v)
{
    m_nVelocity = v;
}

void Moveable::set_position(const Matrix4x4 m)
{
    m_pElement->set_transform(m);
}

void Moveable::set_position(const Vector3D& v)
{
    Matrix4x4 m;
    m_pElement->set_transform(m);
    m_pElement->translate(v);
}

NPC::NPC(SceneNode* element, MoveableSubscriber* subscriber)
    : Moveable(element, subscriber)
    , m_nCooldown(0)
    , m_nThrottle(0.4)
    , m_nHealth(100)
    , m_nCooldownReset(ENEMY_COOLDOWN)
{
    // DEFAULTS
    std::fill(&m_eAiming[0], &m_eAiming[4], false);
    std::fill(&m_eMoving[0], &m_eMoving[4], false);

    // Set up joint references for animation
    m_pJoints[NPC::JOINT_CENTRE]         = static_cast<JointNode*>(m_pElement->find("cog"));
    m_pJoints[NPC::JOINT_UPPER_CENTRE]   = static_cast<JointNode*>(m_pElement->find("armcog"));
    m_pJoints[NPC::JOINT_RIGHT_SHOULDER] = static_cast<JointNode*>(m_pElement->find("rshoulder"));
    m_pJoints[NPC::JOINT_RIGHT_ELBOW]    = static_cast<JointNode*>(m_pElement->find("relbow"));
    m_pJoints[NPC::JOINT_RIGHT_WRIST]    = static_cast<JointNode*>(m_pElement->find("rwrist"));
    m_pJoints[NPC::JOINT_RIGHT_HIP]      = static_cast<JointNode*>(m_pElement->find("rhip"));
    m_pJoints[NPC::JOINT_RIGHT_KNEE]     = static_cast<JointNode*>(m_pElement->find("rknee"));
    m_pJoints[NPC::JOINT_LEFT_SHOULDER]  = static_cast<JointNode*>(m_pElement->find("lshoulder"));
    m_pJoints[NPC::JOINT_LEFT_ELBOW]     = static_cast<JointNode*>(m_pElement->find("lelbow"));
    m_pJoints[NPC::JOINT_LEFT_WRIST]     = static_cast<JointNode*>(m_pElement->find("lwrist"));
    m_pJoints[NPC::JOINT_LEFT_HIP]       = static_cast<JointNode*>(m_pElement->find("lhip"));
    m_pJoints[NPC::JOINT_LEFT_KNEE]      = static_cast<JointNode*>(m_pElement->find("lknee"));

    for (int i = 0; i < 11; i++) {
        if (m_pJoints[i] == 0) {
            std::cerr << "Shit's up, couldn't load all joints!" << std::endl;
        }
    }

    m_pJoints[NPC::JOINT_CENTRE]->set_axis('y');
    m_pJoints[NPC::JOINT_UPPER_CENTRE]->set_axis('y');

    // Gangster style shooting
    m_pJoints[NPC::JOINT_RIGHT_WRIST]->set_axis('y');
    m_pJoints[NPC::JOINT_LEFT_WRIST]->set_axis('y');

    // Set size of radius
    m_pElement->set_radius(3);

    // Ensure I default to no AI
    m_pAI = 0;

}

NPC::~NPC()
{

}

bool NPC::damage(int nDamage)
{

    m_nHealth -= nDamage;

    return m_nHealth <= 0;

}

Moveable* NPC::IsHit(const Point3D& p, double r)
{

    Point3D pCentre;
    m_pElement->get_centre(pCentre);

    double nRadius = r + m_pElement->get_radius(); //std::max(r, m_pElement->get_radius());
    double nDifference = (p - pCentre).length();

    if (nRadius > nDifference) {
        return this;
    }

    return (Moveable*)0;

}

void NPC::getCentre(Point3D& p)
{
    m_pElement->get_centre(p);
}

bool NPC::move(AI::Direction ePrimary, AI::Direction eSecondary)
{

    // Reset movement to nothing
    // std::fill(&m_eMoving[0], &m_eMoving[4], false);
    bool bMove[4] = {false, false, false, false};

    int nRand = rand();
    int nDiff = (nRand % 2 == 0) ? 1 : 3;

    // Usually go right way
    if (nRand > WATERMARK_HIGH || (m_pAI && m_pAI->isAuto())) {
        // Set proper direction to true
        bMove[ePrimary] = true;
        bMove[eSecondary] = true;
    } else if (nRand > WATERMARK_LOW) {
        // Sometimes go diagonal
        bMove[ePrimary] = true;
    } else {
        // Lastly, go sideways
        // bMove[((ePrimary + nDiff) % 4)] = true;
        bMove[((ePrimary + nDiff) % 4)] = true;
    }

    for (int i = (int)ePrimary; i < (int)ePrimary + 4; i++) {
        setMoving((Direction)(i % 4) , bMove[i % 4]);
    }

    return true;
}

void NPC::setMoving(Direction d, bool b)
{

    bool bWasMoving = isMoving();
    bool bRequireUpdate = !(m_eMoving[d] == b);

    m_eMoving[d] = b;

    if (bWasMoving != isMoving()) {
        updateMovingAnimation();
    }
    if (bRequireUpdate) {
        updateMovingDirection();
    }
}

void NPC::setAiming(Direction d, bool b)
{

    bool bWasAiming = isAiming();
    bool bRequireUpdate = !(m_eAiming[d] == b);

    m_eAiming[d] = b;

    if (bWasAiming != isAiming()) {
        updateAimingAnimation();
    }
    if (bRequireUpdate) {
        updateAimingDirection();
    }
}

bool NPC::isMoving()
{
    return m_eMoving[0] || m_eMoving[1] || m_eMoving[2] || m_eMoving[3];
}

bool NPC::isAiming()
{
    return m_eAiming[0] || m_eAiming[1] || m_eAiming[2] || m_eAiming[3];
}

void NPC::tick()
{

    OutputDir eOut = getOutputDirection();

    double x = 0, z = 0;

    // Move Me
    switch (eOut) {
    case D:
        z = 1;
        break;
    case DR:
        z = .7;
        x = .7;
        break;
    case R:
        x = 1;
        break;
    case UR:
        x = .7;
        z = -.7;
        break;
    case U:
        z = -1;
        break;
    case UL:
        z = -.7;
        x = -.7;
        break;
    case L:
        x = -.7;
        break;
    case DL:
        x = -.7;
        z = .7;
        break;
    default:
        break;
    }

    Vector3D v(x, 0, z);
    v = m_nThrottle * v;

    // Move person
    m_pElement->translate(v);

    // Allow child to override collision rules
    Moveable* m = getCollidingMoveable();

    // Allow child to override collision action
    if ((m_pAI && !m_pAI->isAuto()) || !(isNPC())) {
        doCollisionAction(m, v);
    }

    // Allow child to override particular behaviour
    doUniqueAction();

    // See if we need to switch to normal AI
    if (m_pAI && m_pAI->isAuto()) {
        Point3D p;
        getCentre(p);
        if (abs(p[0]) < 50 && abs(p[2]) < 50) {
            revertToAI();
        }
    }

    // Update my animations
    m_pElement->tick();
}

Moveable* NPC::getCollidingMoveable()
{
    Point3D p;
    m_pElement->get_centre(p);

    return m_pSubscriber->DetectCollision(p, m_pElement->get_radius(), this);
}

void NPC::doCollisionAction(Moveable* m, const Vector3D& v)
{
    if (!m) {
        if (v[0] == 0)
            return;
        return;
    }

    switch (m->getType()) {

    case TYPE_OBSTACLE: { // bounce off
        Point3D pMe, pThem;
        m->getCentre(pThem);
        getCentre(pMe);
        Vector3D v = (pMe - pThem);
        v[1] = 0;
        m_pElement->translate(.1 * v);
        break;
    }
    case TYPE_PC: // Kill the human
        if (isNPC()) {
            Point3D pHit;
            dynamic_cast<NPC*>(m)->getCentre(pHit);
            m_pSubscriber->DamageEnemy((NPC*)(m), 10000, pHit);
        }
        break;

    default:
        break;
        // Ignore for now?
    }

}

NPC::OutputDir NPC::getOutputDirection()
{

    // Check diagonals
    for (int i = 0; i < 4; i++) {
        if (m_eMoving[i] && m_eMoving[(i + 1) % 4]) {
            return (OutputDir)((2 * i) + 1);
        }
    }

    // Fall back to directions
    for (int i = 0; i < 4; i++) {
        if (m_eMoving[i]) {
            return (OutputDir)(2 * i);
        }
    }

    return NO_MOVEMENT;
}

NPC::OutputDir NPC::getTargetDirection()
{

    // Check diagonals
    for (int i = 0; i < 4; i++) {
        if (m_eAiming[i] && m_eAiming[(i + 1) % 4]) {
            return (OutputDir)((2 * i) + 1);
        }
    }

    // Fall back to directions
    for (int i = 0; i < 4; i++) {
        if (m_eAiming[i]) {
            return (OutputDir)(2 * i);
        }
    }

    return NO_MOVEMENT;
}

void NPC::walk_gl()
{
    Moveable::walk_gl();
}


void NPC::updateAimingDirection(bool bCascade)
{

    int d = isAiming() ? getTargetDirection() : getOutputDirection();

    if (d == NO_MOVEMENT) {
        return;
    }

    // Turn upper body to face the correct direction
    m_pJoints[JOINT_UPPER_CENTRE]->freeze();

    AnnimationFrame* pFrame = new AnnimationFrame();
    pFrame->m_nFrames = 5;
    pFrame->m_nRemainingFrames = 5;
    pFrame->m_bLoopBack = false;
    pFrame->m_nAngle = 45 * d;
    m_pJoints[JOINT_UPPER_CENTRE]->add_frame(pFrame);

    if (!isMoving() && bCascade) {
        updateMovingDirection(false);
    }

}

void NPC::revertToAI()
{

    m_pSubscriber->CreateAI(this);

}

void NPC::updateMovingDirection(bool bCascade)
{
    AnnimationFrame* pFrame;

    int d = isMoving() ? getOutputDirection() : getTargetDirection();

    if (d == NO_MOVEMENT) {
        return;
    }

    // Turn lower body to face the correct direction
    m_pJoints[JOINT_CENTRE]->freeze();
    pFrame = new AnnimationFrame();
    pFrame->m_nFrames = 5;
    pFrame->m_nRemainingFrames = 5;
    pFrame->m_bLoopBack = false;
    pFrame->m_nAngle = 45 * d;
    m_pJoints[JOINT_CENTRE]->add_frame(pFrame);

    if (!isAiming() && bCascade) {
        updateAimingDirection(false);
    }

}

void NPC::updateAimingAnimation()
{

    AnnimationFrame* pFrame;

    m_pJoints[JOINT_RIGHT_SHOULDER]->freeze();
    m_pJoints[JOINT_RIGHT_WRIST]->freeze();
    m_pJoints[JOINT_LEFT_SHOULDER]->freeze();
    m_pJoints[JOINT_LEFT_WRIST]->freeze();

    if (isAiming()) {

        AnnimationFrame* pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 70;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_RIGHT_SHOULDER]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = -90;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_RIGHT_WRIST]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 70;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_LEFT_SHOULDER]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 90;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_LEFT_WRIST]->add_frame(pFrame);

    } else {

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 0;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_RIGHT_SHOULDER]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 0;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_RIGHT_WRIST]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 0;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_LEFT_SHOULDER]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 0;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_LEFT_WRIST]->add_frame(pFrame);
    }

}

void NPC::updateMovingAnimation()
{

    AnnimationFrame* pFrame;
    m_pJoints[JOINT_LEFT_HIP]->freeze();
    m_pJoints[JOINT_LEFT_KNEE]->freeze();
    m_pJoints[JOINT_RIGHT_HIP]->freeze();
    m_pJoints[JOINT_RIGHT_KNEE]->freeze();

    if (isMoving()) { // Enqueue all the running frames
        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 45;
        pFrame->m_nFrames = JOG_FRAME_LENGTH / 2;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH / 2;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_LEFT_HIP]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = -15;
        pFrame->m_nFrames = JOG_FRAME_LENGTH / 2;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH / 2;
        pFrame->m_bLoopBack = false;
        m_pJoints[JOINT_LEFT_KNEE]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = -45;
        pFrame->m_nFrames = JOG_FRAME_LENGTH;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH;
        pFrame->m_bLoopBack = true;

        m_pJoints[JOINT_LEFT_HIP]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = -105;
        pFrame->m_nFrames = JOG_FRAME_LENGTH;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH;
        pFrame->m_bLoopBack = true;

        m_pJoints[JOINT_LEFT_KNEE]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 75;
        pFrame->m_nFrames = JOG_FRAME_LENGTH;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH;
        pFrame->m_bLoopBack = true;

        m_pJoints[JOINT_LEFT_HIP]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = -30;
        pFrame->m_nFrames = JOG_FRAME_LENGTH;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH;
        pFrame->m_bLoopBack = true;
        m_pJoints[JOINT_LEFT_KNEE]->add_frame(pFrame);

        // Right Leg inital step backward

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = -10;
        pFrame->m_nFrames = JOG_FRAME_LENGTH / 2;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH / 2;
        pFrame->m_bLoopBack = false;

        m_pJoints[JOINT_RIGHT_HIP]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 0;
        pFrame->m_nFrames = JOG_FRAME_LENGTH / 2;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH / 2;
        pFrame->m_bLoopBack = false;

        m_pJoints[JOINT_RIGHT_KNEE]->add_frame(pFrame);

        // Right leg continuous motion

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 75;
        pFrame->m_nFrames = JOG_FRAME_LENGTH;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH;
        pFrame->m_bLoopBack = true;

        m_pJoints[JOINT_RIGHT_HIP]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = -30;
        pFrame->m_nFrames = JOG_FRAME_LENGTH;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH;
        pFrame->m_bLoopBack = true;
        m_pJoints[JOINT_RIGHT_KNEE]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = -45;
        pFrame->m_nFrames = JOG_FRAME_LENGTH;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH;
        pFrame->m_bLoopBack = true;

        m_pJoints[JOINT_RIGHT_HIP]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = -105;
        pFrame->m_nFrames = JOG_FRAME_LENGTH;
        pFrame->m_nRemainingFrames = JOG_FRAME_LENGTH;
        pFrame->m_bLoopBack = true;

        m_pJoints[JOINT_RIGHT_KNEE]->add_frame(pFrame);

    } else { // Enqueue resting frames
        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 0;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;

        m_pJoints[JOINT_LEFT_HIP]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 0;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;

        m_pJoints[JOINT_LEFT_KNEE]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 0;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;

        m_pJoints[JOINT_RIGHT_HIP]->add_frame(pFrame);

        pFrame = new AnnimationFrame();
        pFrame->m_nAngle = 0;
        pFrame->m_nFrames = 5;
        pFrame->m_nRemainingFrames = 5;
        pFrame->m_bLoopBack = false;

        m_pJoints[JOINT_RIGHT_KNEE]->add_frame(pFrame);
    }


}

const Point3D NPC::get_gun_nozzle()
{
    Point3D p;
    m_pJoints[JOINT_RIGHT_WRIST]->get_centre(p);

    return p;
}

PistolNPC::PistolNPC(SceneNode* element, MoveableSubscriber* subscriber)
    : NPC(element, subscriber)
    , m_nCooldown(ENEMY_COOLDOWN)
{
    m_nThrottle = 0.2;
}

PistolNPC::~PistolNPC() { }

void PistolNPC::doUniqueAction()
{

    // Cooldown gun
    if (m_nCooldown) {
        m_nCooldown--;
    } else {

        m_nCooldown = m_nCooldownReset * (rand() / ((double)RAND_MAX) * 4);
        m_pSubscriber->CreateBullet(get_gun_nozzle(), getOutputDirection() * 45 , this);
    }
}


PC::PC(SceneNode* element, MoveableSubscriber* subscriber)
    : NPC(element, subscriber)
    , m_bRightSideShot(0)
    , m_bHumanControlled(0)
    , m_pShield(0)
{
    m_pNozzle[0] = m_pElement->find("ruzi_nozzle");
    m_pNozzle[1] = m_pElement->find("luzi_nozzle");

    m_nThrottle = 0.8;

    m_nHealth = 100;

    // Set size of radius (smaller than enemies)
    m_pElement->set_radius(2);

    m_pShield = m_pSubscriber->RequestShield(MoveableSubscriber::MAJOR);
    m_pElement->add_child(m_pShield->getRoot());
}

PC::~PC()
{
}

void PC::tick()
{

    // If moving
    NPC::tick();

    // Cooldown gun
    if (m_nCooldown) {
        m_nCooldown--;
    }

    // If moving
    if (isAiming() && !m_nCooldown) {

        m_pSubscriber->CreateBullet(get_gun_nozzle(), getTargetDirection() * 45, this);
        m_pSubscriber->PlaySFX(MoveableSubscriber::SFX_BULLET);
        m_nCooldown = PLAYER_COOLDOWN;
    }

    // tick shield
    if (m_pShield) {
        m_pShield->tick();

        // Do I need a new shield?
        if (!m_pShield->getTTL()) {
            // Minor?
            if (m_pShield->isMajor()) {
                m_pElement->remove_child(m_pShield->getRoot());
                delete m_pShield;
                m_pShield = m_pSubscriber->RequestShield(MoveableSubscriber::MINOR);
                m_pElement->add_child(m_pShield->getRoot());
            } else {
                m_pElement->remove_child(m_pShield->getRoot());
                delete m_pShield;
                m_pShield = 0;
            }
        }
    }
}

Moveable* PC::getCollidingMoveable()
{
    Point3D p;
    m_pElement->get_centre(p);

    return m_pSubscriber->DetectCollision(p, m_pElement->get_radius(), this);
}

void PC::doCollisionAction(Moveable* m, const Vector3D& v)
{

    NPC::doCollisionAction(m, v);

}

const Point3D PC::get_gun_nozzle()
{
    Point3D p;

    if (m_bRightSideShot) {
        m_pNozzle[0]->get_centre(p);
    } else {
        m_pNozzle[1]->get_centre(p);
    }

    m_bRightSideShot = !m_bRightSideShot;

    return p;
    // m_pJoints[JOINT_RIGHT_WRIST]->get_transform() ;
}

void PC::walk_gl()
{

    NPC::walk_gl();

    //if (m_pShield) {
    //
    // m_pShield->walk_gl();
    //}
}

Moveable* PC::IsHit(const Point3D& p, double r)
{

    Moveable* m = 0;
    if (m_pShield) {
        m = m_pShield->IsHit(p, r);
    }

    return (m) ? m : NPC::IsHit(p, r);

}

void PC::die()
{
    m_nHealth = 100;

    Point3D p;
    m_pElement->get_centre(p);

    m_pElement->translate(Point3D(0, 0, 0) - p);

    // Set size of radius (smaller than enemies)
    m_pElement->set_radius(2);

    m_pShield = m_pSubscriber->RequestShield(MoveableSubscriber::MAJOR);
    m_pElement->add_child(m_pShield->getRoot());

}
