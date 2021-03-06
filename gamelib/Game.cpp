#include "Game.h"
#include "Object.h"
#include "SceneLua.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/string_cast.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <GLES3/gl3.h>

#include <iostream>
#include <cstdlib>
#include <sstream>

// #include "SoundManager.h"
// #include "joystick/include.h"

static unsigned    BULLET_CAP = 128;
static int    ENEMY_CAP = 64;

static int    LITTLE_PARTICLES = 92;
static int    MEDIUM_PARTICLES = 116;
static int    BIGGER_PARTICLES = 128;
static int    SPARKS_PARTICLES = 156;
static int    PARTICLES       = 156;

static int    MOBS    = 2;
static int    MOB_SIZE = 12;
static unsigned    ENEMY_WATERMARK = 3;

static int    LIFE_CAP = 10;

using namespace glm;

static bool debug = false;

Game::Game(int argc, char** argv)
    : m_pLevel(0)
    , m_Objs()
    , m_PCs()
    , m_NPCs()
    , m_Bullets()
    , m_AIs()
    , m_Particles()
    , m_EnemyIndex()
    , m_pDefaultBullet(0)
    , m_pDefaultEnemy(0)
    , m_nBulletCounter(0)
    , m_nEnemyCounter(0)
    , m_nKeyboardPlayer(0)
    , m_bRunning(true)
    , m_nPlayers(1)
    , m_nLives(LIFE_CAP)
    , m_nScore(0)
    , m_frames(0)
    , m_movements(0)
    , m_gfx()
{
    int nPlayers = 1;
    // Import the level
    m_pLevel = new Level(import_lua("models/level.lua"), this);
    std::cout << "Creating " << nPlayers << " players." << std::endl;

    // Set up shield lua files
    m_pShield[0] = import_lua("models/shield.lua");
    m_pShield[1] = import_lua("models/shield_minor.lua");

    if (nPlayers >= 1) {
        PC* pc0 = new PC(import_lua("models/character1.lua"), this);
        m_PCs.push_back(pc0);
        pc0->setHumanControlled(true);
    }

    if (nPlayers >= 2) {
        PC* pc1 = new PC(import_lua("models/character2.lua"), this);
        m_PCs.push_back(pc1);
        pc1->setHumanControlled(true);
    }

    if (nPlayers >= 3) {
        PC* pc2 = new PC(import_lua("models/character3.lua"), this);
        m_PCs.push_back(pc2);
        pc2->setHumanControlled(true);
    }

    if (nPlayers >= 4) {
        PC* pc3 = new PC(import_lua("models/character4.lua"), this);
        m_PCs.push_back(pc3);
        pc3->setHumanControlled(true);
    }

    if (argc > 1) {
        debug = true;
    }

    if (debug) {
        MOBS = 0;
        ENEMY_WATERMARK = 0;
    }

    m_Bullets.reserve(BULLET_CAP);
    m_NPCs.reserve(ENEMY_CAP);

    // Copies of stuff that will be re-used
    m_pModels[MODEL_ENEMY_SWORD] =       import_lua("models/enemy_sword.lua");
    m_pModels[MODEL_ENEMY_SWORD_BOSS] =  import_lua("models/enemy_sword.lua");
    m_pModels[MODEL_ENEMY_SWORD_BOSS]->colour(Colour(.2, .2, 2));
    m_pModels[MODEL_ENEMY_SWORD_FAST] =       import_lua("models/enemy_sword_fast.lua");
    m_pModels[MODEL_ENEMY_PISTOL] =      import_lua("models/enemy_pistol.lua");
    m_pModels[MODEL_ENEMY_PISTOL_BOSS] = import_lua("models/enemy_pistol.lua");
    m_pModels[MODEL_ENEMY_PISTOL_BOSS]->colour(Colour(.4, .4, 0));
    m_pModels[MODEL_BULLET] =            import_lua("models/bullet.lua");
    m_pModels[MODEL_ENEMY_BULLET] =      import_lua("models/enemy_bullet.lua");

    // Import a bullet
    // m_pDefaultBullet = Bullet::CreateBullet(this, (Moveable*)0);
    // Import an enemy
    m_pDefaultEnemy = new NPC(import_lua("models/enemy.lua"), this);

    m_nParticleIndex[0] = 0;
    m_nParticleIndex[1] = LITTLE_PARTICLES;
    m_nParticleIndex[2] = MEDIUM_PARTICLES;
    m_nParticleIndex[3] = BIGGER_PARTICLES;

    // Create all particles
    for (int i = 0; i < PARTICLES; i++) {
        Particle* p;

        if (i < LITTLE_PARTICLES) {
            p = new Particle(import_lua("models/chunk_little.lua"), this);
        } else if (i < MEDIUM_PARTICLES) {
            p = new Particle(import_lua("models/chunk_medium.lua"), this);
        } else if (i < BIGGER_PARTICLES) {
            p = new Particle(import_lua("models/chunk_bigger.lua"), this);
        } else if (i < SPARKS_PARTICLES) {
            p = new Spark(import_lua("models/chunk_sparks.lua"), this);
        }

        p->set_dead(true);
        m_Particles.push_back(p);
    }

    // Create AI
    m_AIs.push_back(new AutoAI(AI::UP));
    m_AIs.push_back(new AutoAI(AI::LEFT));
    m_AIs.push_back(new AutoAI(AI::DOWN));
    m_AIs.push_back(new AutoAI(AI::RIGHT));

    // Create enemies
    for (int i = 0; i < MOBS; i++) {
        CreateMob();
    }

    // JOYSTICK STUFF
    //
    //enable joysticks (yes, video is required)
    /*
    if(initSdlForJoysticks() != 0){
        printf("SDL can't init.\n");
        exit(1);
    }

    m_pJoystick[0] = 0;
    m_pJoystick[1] = 0;
    m_nJoystickPlayer[0] = -1;
    m_nJoystickPlayer[1] = -1;

    int numJoys = getNumJoysticks();
    printf("Number of joysticks found: %d\n", numJoys);

    for(int i = 0; i < numJoys && i < 2 ; i++){
        printf("Joystick %d: %s\n", i, getJoystickName(i));
        m_pJoystick[i] = loadJoystick(i);
        m_nJoystickPlayer[i] = (i + 1) % nPlayers;
    }
    */
}

Game::~Game()
{

}

void Game::play()
{
    //int music = SM.LoadMusic("data/WOM.XM");
    /*
    int music = SM.LoadMusic("data/UNREAL.S3M");

    SM.VolumeMusic(128);
    SM.PlayMusic(music);

    m_nSFX[0] = SM.LoadSound("data/gunshot.wav");
    m_nSFX[1] = SM.LoadSound("data/carom.wav");

    for (int i = 0; i < 14; i++) {
        std::stringstream ss;
        ss << "data/ricochet/h_1metal" << i << ".wav";
        m_nSFX[SFX_CAROM0 + i] = SM.LoadSound(ss.str());
    }

    for (int i = 0; i < 2; i++) {
        std::stringstream ss;
        ss << "data/hit" << i << ".wav";
        std::cout << ss.str() << std::endl;
        m_nSFX[SFX_HIT0 + i] = SM.LoadSound(ss.str());
    }

    m_nSFX[SFX_HIT0] = SM.LoadSound("data/hit0.wav");
    m_nSFX[SFX_HIT1] = SM.LoadSound("data/hit1.wav");

    m_nSFX[SFX_HUMAN_DEATH] = SM.LoadSound("data/player_death.wav");
    m_nSFX[SFX_ROBOT_DEATH] = SM.LoadSound("data/robot_death.wav");
    */
}

// Increment one unit of the time in the game world.
// All characters in motion will move, all bullets will move,
// and collisions will be detected. Returns false if all enemies
// are defeated, or both players are out of lives, true otherwise
bool Game::tick()
{

    // Check joystick
    /*
    JoystickEvent* event;
    while ((event = getJoystickEvent())) {
        int joystickId = event->getJoystickId();
        if(joystickId == m_pJoystick[joystickId]->getId()) {
            m_pJoystick[joystickId]->processEvent(event);
            handleJoystick(m_pJoystick[joystickId], m_PCs[m_nJoystickPlayer[joystickId]]);
        }
    }
    */

    // Now stop if paused
    if (!m_bRunning) {
        return true;
    }

    // Fire AI
    for (auto& ai : m_AIs) {
        ai->tick();
    }

    // Move Everything
    for (auto& pc : m_PCs) {
        pc->tick();
    }

    for (auto& npc : m_NPCs) {
        npc->tick();
    }

    for (unsigned int i = 0; i < m_Bullets.size(); i++) {
        if (i < m_Bullets.size()) {
            (m_Bullets[i])->tick();
        }
    }

    // Attempt to move particles
    for (auto& particle : m_Particles) {
        if (!particle->is_dead()) {
            particle->tick();
        }
    }

    // Create some more mobs if we are low on enemies
    while (m_NPCs.size() < ENEMY_WATERMARK) {
        for (int i = 0; i < MOBS; i++) {
            CreateMob();
        }
    }

    // Make sure doors are opened and closed
    for (int i = 0; i < 4; i++) {
        (m_AIs[i]->isActive()) ? m_pLevel->openDoor((Level::Door)i) : m_pLevel->closeDoor((Level::Door)i);
    }

    m_pLevel->tick();

    if (!m_nLives) {
        std::cout << "Game over." << std::endl;
        std::cout << "  -> Final score: " << m_nScore << std::endl;
    }

    return m_nLives;
}

void Game::input(Action action, int nPlayer)
{

    if ((unsigned int)nPlayer > m_PCs.size()) {
        return;
    }

    PC* pc = m_PCs[nPlayer];

    if (!pc->isHumanControlled()) {
        return;
    }

    switch (action) {

    // Positive movement and aiming
    case ACTION_MOVE_LEFT:
        pc->setMoving(NPC::LEFT, true);
        break;
    case ACTION_MOVE_UP:
        pc->setMoving(NPC::UP, true);
        break;
    case ACTION_MOVE_RIGHT:
        pc->setMoving(NPC::RIGHT, true);
        break;
    case ACTION_MOVE_DOWN:
        pc->setMoving(NPC::DOWN, true);
        break;
    case ACTION_SHOOT_LEFT:
        pc->setAiming(NPC::LEFT, true);
        break;
    case ACTION_SHOOT_RIGHT:
        pc->setAiming(NPC::RIGHT, true);
        break;
    case ACTION_SHOOT_UP:
        pc->setAiming(NPC::UP, true);
        break;
    case ACTION_SHOOT_DOWN:
        pc->setAiming(NPC::DOWN, true);
        break;

    // Stops
    case ACTION_STOP_MOVE_LEFT:
        pc->setMoving(NPC::LEFT, false);
        break;
    case ACTION_STOP_MOVE_UP:
        pc->setMoving(NPC::UP, false);
        break;
    case ACTION_STOP_MOVE_RIGHT:
        pc->setMoving(NPC::RIGHT, false);
        break;
    case ACTION_STOP_MOVE_DOWN:
        pc->setMoving(NPC::DOWN, false);
        break;
    case ACTION_STOP_SHOOT_LEFT:
        pc->setAiming(NPC::LEFT, false);
        break;
    case ACTION_STOP_SHOOT_RIGHT:
        pc->setAiming(NPC::RIGHT, false);
        break;
    case ACTION_STOP_SHOOT_UP:
        pc->setAiming(NPC::UP, false);
        break;
    case ACTION_STOP_SHOOT_DOWN:
        pc->setAiming(NPC::DOWN, false);
        break;

    case ACTION_TOGGLE_PAUSE:
        break;

    }

    m_movements++;
}

/*
void Game::handleJoystick(Joystick* joy, PC * pc){

    if (!pc->isHumanControlled()) {
        return;
    }

    if (joy->getButtonDown(Joystick::START)) {
        togglePause();
    }

    //get analog stick values
    float x, y;
    joy->getDPad(x, y);
    if (x > 0) {
        pc->setMoving(NPC::RIGHT, true);
        pc->setMoving(NPC::LEFT, false);
    } else if (x < 0) {
        pc->setMoving(NPC::LEFT, true);
        pc->setMoving(NPC::RIGHT, false);
    } else {
        pc->setMoving(NPC::LEFT, false);
        pc->setMoving(NPC::RIGHT, false);
    }

    if (y > 0) {
        pc->setMoving(NPC::DOWN, true);
        pc->setMoving(NPC::UP, false);
    } else if (y < 0) {
        pc->setMoving(NPC::UP, true);
        pc->setMoving(NPC::DOWN, false);
    } else {
        pc->setMoving(NPC::UP, false);
        pc->setMoving(NPC::DOWN, false);
    }

    pc->setAiming(NPC::DOWN,  joy->getButtonDown(Joystick::B) ? true : false);
    pc->setAiming(NPC::RIGHT, joy->getButtonDown(Joystick::A) ? true : false);
    pc->setAiming(NPC::UP,    joy->getButtonDown(Joystick::X) ? true : false);
    pc->setAiming(NPC::LEFT,  joy->getButtonDown(Joystick::Y) ? true : false);

}
*/

unsigned fbo;
unsigned depthTextureID;


void Game::init_gl()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // DISPLAY LIST
    SceneNode::setupDL();

    m_gfx.initialize();


    glGenTextures(1, &depthTextureID);
    glBindTexture(GL_TEXTURE_2D, depthTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 512, 512, 0, GL_DEPTH_COMPONENT,	GL_FLOAT, 0);


    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0/*mipmap level*/);

    std::cout << "Framebuffer Status " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << " Complete is " << GL_FRAMEBUFFER_COMPLETE << std::endl;
}

glm::mat4 camera(const glm::vec3& trans, const glm::vec2& rot)
{
    glm::mat4 view = glm::translate(glm::mat4(1.0f), trans);
    view = glm::rotate(view, rot.y, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, rot.x, glm::vec3(0.0f, 1.0f, 0.0f));
    return view;
}


void Game::walk_gl()
{
    glDisable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!m_pLevel)
        return;

    m_frames++;

    vec3 lightPosition(0, 10, 2);
    m_gfx.setUniform(Uniform::LightPosition, lightPosition);

    vec2 cameraAngle(0, radians(70.0f));
    vec3 cameraTranslation(0, 0, -40);
    auto cameraTransform = camera(cameraTranslation, cameraAngle);
    auto perspectiveProjection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 1.f, 100.f) * cameraTransform;

    if (debug) {
        //cameraAngle = vec2(radians(m_frames / 4.0f), radians(45.0f));
        //cameraTranslation = vec3(0, -1.0f, -5.0f);
        cameraTransform = lookAt(lightPosition, vec3(0,0,2), vec3(0,0,-1));
        perspectiveProjection = glm::perspective(glm::pi<float>() * .825f, 4.0f / 3.0f, 1.f, 100.f) * cameraTransform;
    }

    m_gfx.setUniformMatrix(Uniform::Perspective, perspectiveProjection);

    vec4 openglEyePosition(0, 0, 0, 1);
    vec4 worldEyePosition = inverse(cameraTransform) * openglEyePosition;
    vec3 eyePosition(worldEyePosition[0], worldEyePosition[1], worldEyePosition[2]);
    m_gfx.setUniform(Uniform::EyePosition, eyePosition);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    drawScene();

    glBindTexture(GL_TEXTURE_2D, depthTextureID);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    drawScene();
}

void Game::drawScene()
{
    auto modelview = glm::mat4();
    m_gfx.setUniformMatrix(Uniform::Modelview, modelview);

    m_pLevel->walk_gl2(m_gfx);

    auto scaleDown = glm::scale(glm::mat4x4(), glm::vec3(.2f, .2f, .2f));
    auto translateUp = glm::translate(glm::mat4x4(), glm::vec3(0.0f, 7.0f, 0.0f));

    modelview = modelview * scaleDown * translateUp;
    m_gfx.setUniformMatrix(Uniform::Modelview, modelview);

    for (auto& pc : m_PCs)
        pc->walk_gl2(m_gfx);

    for (auto& npc : m_NPCs)
        npc->walk_gl2(m_gfx);

    for (auto& bullet : m_Bullets)
        bullet->walk_gl2(m_gfx);

    for (auto& particle : m_Particles)
        if (!particle->is_dead())
            particle->walk_gl2(m_gfx);
}

void Game::CreateBullet(const vec3& origin, float dDegrees, NPC* source)
{
    int nQuantity = 1;

    if (m_Bullets.size() + nQuantity > BULLET_CAP) {
        return;
    }

    Bullet* b;
    for (int i = 0; i < nQuantity; i++) {

        if (source->isNPC()) {
            b = new Bullet((m_pModels[MODEL_ENEMY_BULLET])->clone(), this, source);
            b->set_velocity(.5);
        } else {
            b = new Bullet((m_pModels[MODEL_BULLET])->clone(), this, source);
            b->set_velocity(3);
        }

        b->set_joint();
        b->set_direction(dDegrees);
        b->set_position(origin - vec3(0, 0, 0));
        b->set_dead(false);

        m_Bullets.push_back(b);

        // Muzzle flash
        CreateParticles(SIZE_SPARKS, 1, origin - vec3(0, 0, 0));
    }
}

void Game::DeleteBullet(Bullet* pBullet)
{
    // Find and delete bullet
    for (BulletList::iterator it = m_Bullets.begin();
            it != m_Bullets.end(); it++) {

        if (*it == pBullet) {
            delete *it;
            m_Bullets.erase(it);
            return;
        }
    }
}

#define LEVEL_MAX_X 80
#define LEVEL_MAX_Z 60

Moveable* Game::DetectCollision(const vec3& p, float r, Moveable* pExcluded)
{
    Moveable* m = 0;

    // Check for collision with box
    for (ObjList::iterator it = m_Objs.begin(); it != m_Objs.end(); it++) {
        if (!(*it)->is_dead()) {
            m = (*it)->IsHit(p, r);
            if (m && m != pExcluded) {
                return m;
            }
        }
    }

    // Check for collision with enemy
    for (EnemyList::iterator it = m_NPCs.begin(); it != m_NPCs.end(); it++) {
        m = (*it)->IsHit(p, r);
        if (m && m != pExcluded) {
            return m;
        }
    }

    // Check for collision with player
    for (PCList::iterator it = m_PCs.begin(); it != m_PCs.end(); it++) {
        m = (*it)->IsHit(p, r);
        if (m && m != pExcluded) {
            return m;
        }
    }

    return m;
}

AISubscriber* Game::CreateEnemy(int x, int z)
{
    //  NPC * npc = new NPC();
    int nRand = rand() / (RAND_MAX / 40);
    NPC* npc;

    if (nRand <= 12) {
        npc = new PistolNPC((m_pModels[MODEL_ENEMY_PISTOL])->clone(), this);
        npc->setHealth(25 * m_PCs.size());
    } else if (nRand <= 15) {
        npc = new PistolNPC((m_pModels[MODEL_ENEMY_PISTOL_BOSS])->clone(), this);
        npc->setHealth(125 * m_PCs.size());
        npc->setCooldown(5);
    } else if (nRand <= 28) {
        npc = new NPC((m_pModels[MODEL_ENEMY_SWORD])->clone(), this);
        npc->setHealth(50 * m_PCs.size());
    } else if (nRand <= 38) {
        npc = new NPC((m_pModels[MODEL_ENEMY_SWORD_FAST])->clone(), this);
        npc->setHealth(10 * m_PCs.size());
        npc->setThrottle(.5);
    } else {
        npc = new NPC((m_pModels[MODEL_ENEMY_SWORD_BOSS])->clone(), this);
        npc->setHealth(200 * m_PCs.size());
    }

    npc->set_dead(false);

    int xDiff = (int)(((float)rand() / (float)RAND_MAX - 0.5) * 40); //+ 70;
    int zDiff = (int)(((float)rand() / (float)RAND_MAX - 0.5) * 30); //+ 70;

    npc->set_position(glm::vec3(x + xDiff, 0, z + zDiff));

    m_NPCs.push_back(npc);

    return npc;

}

void Game::DamageEnemy(NPC* pNPC, int nDamage, const vec3& p3d)
{
    glm::vec3 v = p3d - vec3(0, 0, 0);
    CreateParticles(SIZE_LITTLE, 2, v);
    CreateParticles(SIZE_SPARKS, 3, v);

    NPC* npc = pNPC;

    if (npc->damage(nDamage)) {

        if (npc->isNPC()) {

            // If dead
            npc->set_dead(true);
            AI* ai = npc->getAI();
            if (ai) {
                ai->remove(npc);

                // Delete ai if not auto
                if (!ai->isAuto() && !ai->isActive()) {
                    DeleteAI(ai);
                }
            }

            for (EnemyList::iterator it = m_NPCs.begin();
                    it != m_NPCs.end(); it++) {
                if (*it == pNPC) {
                    m_NPCs.erase(it);
                    break;
                }
            }

            if (npc->getType() == Moveable::TYPE_NPC) {
                CreateParticles(SIZE_BIGGER, 1, v);
            }

            delete npc;

            // Enemy is destroyed, show explosion effects
            CreateParticles(SIZE_MEDIUM, 2, v);

            // SM.PlaySound(m_nSFX[SFX_ROBOT_DEATH]);

            // Score
            m_nScore++;

            if (!(m_nScore % 10)) {
                std::cout << m_nScore << " enemies defeated." << std::endl;
            }

        } else {
            // The player was killed
            std::cout << "Player killed. " << --m_nLives << " remaining." << std::endl;
            PC* pc = (PC*)npc;
            pc->die();
            // SM.PlaySound(m_nSFX[SFX_HUMAN_DEATH]);
        }
    }
}

void Game::CreateParticles(ParticleSize eSize, int nQuantity, const glm::vec3& v)
{

    for (int i = 0; i < nQuantity; i++) {
        Particle* p = m_Particles[m_nParticleIndex[eSize]];

        switch (eSize) {
        case SIZE_LITTLE:
            m_nParticleIndex[eSize] = (m_nParticleIndex[eSize] + 1) % LITTLE_PARTICLES;
            break;
        case SIZE_MEDIUM:
            m_nParticleIndex[eSize] = (m_nParticleIndex[eSize] + 1 < MEDIUM_PARTICLES) ?  m_nParticleIndex[eSize] + 1 : LITTLE_PARTICLES;
            break;
        case SIZE_BIGGER:
            m_nParticleIndex[eSize] = (m_nParticleIndex[eSize] + 1 < BIGGER_PARTICLES) ?  m_nParticleIndex[eSize] + 1 : MEDIUM_PARTICLES;
            break;
        case SIZE_SPARKS:
            m_nParticleIndex[eSize] = (m_nParticleIndex[eSize] + 1 < PARTICLES) ?  m_nParticleIndex[eSize] + 1 : BIGGER_PARTICLES;
            break;
        }

        p->set_position(v);
        p->reset();
        p->set_dead(false);
    }
}

void Game::CreateMob()
{

    AI* ai = 0;
    int x = 0;
    int z = 0;

    switch (rand() / (RAND_MAX / 4)) {
    case 0:
        z += 100;
        ai = m_AIs[0];
        break;
    case 1:
        x += 120;
        ai = m_AIs[1];
        break;
    case 2:
        z -= 100;
        ai = m_AIs[2];
        break;
    case 3:
        x -= 120;
        ai = m_AIs[3];
        break;
    }

    x += (int)(((float)rand() / (float)RAND_MAX - 0.5) * 5);
    z += (int)(((float)rand() / (float)RAND_MAX - 0.5) * 5);

    if (!ai)
        return;

    // Create AI to auto move
    for (int i = 0; i < MOB_SIZE; i++) {
        ai->addSubscriber(CreateEnemy(x, z));
    }

    //m_AIs.push_back(ai);
    // ai->addTarget(m_PCs[0]);

}

void Game::CreateAI(NPC* npc)
{

    AI* ai = new AI();
    ai->addSubscriber(npc);

    ai->addTarget(m_PCs[rand() / (RAND_MAX / m_PCs.size())]);

    m_AIs.push_back(ai);

}

void Game::DeleteAI(AI* ai)
{

    for (AIList::iterator it = m_AIs.begin();
            it != m_AIs.end(); it++) {
        if (*it == ai) {
            delete *it;
            m_AIs.erase(it);
            return;
        }
    }
}

void Game::PlaySFX(MoveableSubscriber::SFX id)
{

    switch (id) {
    case MoveableSubscriber::SFX_BULLET:
    case MoveableSubscriber::SFX_BLAST:
    case MoveableSubscriber::SFX_BOMB:
        // SM.PlaySound(m_nSFX[0]);
        break;

    case MoveableSubscriber::SFX_CAROM: {
        // int nRand = ((float)(rand() / (float)RAND_MAX) * 16) + SFX_CAROM0;
        // SM.PlaySound(m_nSFX[nRand]);
        break;
    }
    case MoveableSubscriber::SFX_HUMAN_DEATH: {
        // SM.PlaySound(m_nSFX[SFX_HUMAN_DEATH]);
        break;
    }

    case MoveableSubscriber::SFX_ROBOT_HIT:
    case MoveableSubscriber::SFX_ROBOT_DEATH:
        // SM.PlaySound(SFX_ROBOT_DEATH);
        break;
    }
}

void Game::CreateObstacle(Moveable* pObstacle)
{
    m_Objs.push_back(pObstacle);
    pObstacle->set_dead(false);
}

Shield* Game::RequestShield(MoveableSubscriber::ShieldType s)
{

    Shield* shield = 0;
    switch (s) {
    case MoveableSubscriber::MAJOR:
        shield = new Shield(m_pShield[0]->clone(), this);
        shield->setTTL(240);
        shield->setMajor(true);
        break;
    case MoveableSubscriber::MINOR:
        shield = new Shield(m_pShield[1]->clone(), this);
        shield->setTTL(120);
        shield->setMajor(false);
        break;
    }

    return shield;

}

void Game::dumpStats()
{

    std::cout << "Game Stats:" << std::endl;
    std::cout << "-------------------------------------------:" << std::endl;
    std::cout << "Frames Rendered: " << m_frames << std::endl;
    std::cout << "Player Movements: " << m_movements << std::endl;
    std::cout << "-------------------------------------------:" << std::endl;

}

void Game::clearStats()
{
    m_frames = 0;
    m_movements = 0;
}
