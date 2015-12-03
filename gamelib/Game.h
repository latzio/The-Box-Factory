#ifndef SMASHTVGAME_H_
#define SMASHTVGAME_H_

#include "AI.h"
#include "Scene.h"
#include "Object.h"

#include <glm/glm.hpp>

#include <vector>

// This class is to hold all the gamelogic for the BoxFactory Game
// It will have references to the current level and objects within it
// It will store the passable and impassable areas, as well as the
// current status of all activity.
class Game : public MoveableSubscriber {

public:
    // Create a new game
    Game(int nPlayers);
    ~Game();
    NoMoveOrCopy(Game)

    // Initiate the game and start timers
    void play();

    // Increment one unit of the time in the game world.
    // All characters in motion will move, all bullets will move,
    // and collisions will be detected. Returns false if all enemies
    // are defeated, or both players are out of lives, true otherwise
    bool tick();

    // Must be called once before walk_gl to initialize everything GL.
    void init_gl();
    // Draw this game
    void walk_gl();

    enum Action { ACTION_MOVE_LEFT,  ACTION_STOP_MOVE_LEFT,
                  ACTION_MOVE_UP,    ACTION_STOP_MOVE_UP,
                  ACTION_MOVE_RIGHT, ACTION_STOP_MOVE_RIGHT,
                  ACTION_MOVE_DOWN,  ACTION_STOP_MOVE_DOWN,
                  ACTION_SHOOT_LEFT, ACTION_STOP_SHOOT_LEFT,
                  ACTION_SHOOT_RIGHT, ACTION_STOP_SHOOT_RIGHT,
                  ACTION_SHOOT_UP,   ACTION_STOP_SHOOT_UP,
                  ACTION_SHOOT_DOWN, ACTION_STOP_SHOOT_DOWN,
                  ACTION_TOGGLE_PAUSE
                };
    // Input
    void input(Action action, int nPlayer);

    // Create Enemies
    virtual AISubscriber* CreateEnemy(int x, int z);
    virtual void DamageEnemy(NPC* pNPC, int nDamage, const glm::vec3& p3d);

    // Subscribe to game events
    virtual void CreateBullet(const glm::vec3& origin, float dDegrees, NPC* source);
    virtual void DeleteBullet(Bullet* pBullet);

    virtual Moveable* DetectCollision(const glm::vec3& p, float r, Moveable* pExcluded);

    virtual void CreateMob();
    virtual void CreateAI(NPC* npc);
    virtual void DeleteAI(AI* ai);

    virtual Shield* RequestShield(MoveableSubscriber::ShieldType s);

    virtual void CreateParticles(ParticleSize eSize, int nQuantity, const glm::vec3& v);
    virtual void CreateObstacle(Moveable* pObstacle);

    virtual void PlaySFX(MoveableSubscriber::SFX id);

    virtual void pause()
    {
        m_bRunning = false;
    }
    virtual void unpause()
    {
        m_bRunning = true;
    }
    virtual void togglePause()
    {
        m_bRunning = !m_bRunning;
    }


    void dumpStats();
    void clearStats();

protected:

    // This is the level we are playing
    Level* m_pLevel;

    // The various moving objects in the game
    typedef std::vector< Moveable* > ObjList;
    typedef std::vector< PC* > PCList;
    typedef std::vector< Bullet* > BulletList;
    typedef std::vector< NPC* > EnemyList;
    typedef std::vector< AI* > AIList;
    typedef std::vector< Particle* > ParticleList;

    ObjList       m_Objs; // Not drawn at preset, just used by gamelogic
    PCList        m_PCs;
    EnemyList     m_NPCs;
    BulletList    m_Bullets;
    AIList        m_AIs;

    ParticleList  m_Particles;

    std::vector< int > m_EnemyIndex;

    // Some copies to avoid constant lua calls
    Bullet* m_pDefaultBullet;
    NPC* m_pDefaultEnemy;

    int m_nBulletCounter;
    int m_nEnemyCounter;
    int m_nParticleIndex[4];

    // Sound effects
    enum SFX { SFX_BULLET, SFX_BLAST, SFX_BOMB,
               SFX_CAROM0, SFX_CAROM1, SFX_CAROM2, SFX_CAROM3, SFX_CAROM4,
               SFX_CAROM5, SFX_CAROM6, SFX_CAROM7, SFX_CAROM8, SFX_CAROM9,
               SFX_CAROM10, SFX_CAROM11, SFX_CAROM12, SFX_CAROM13, SFX_HIT0 = 17,
               SFX_HIT1 = 18, SFX_HUMAN_DEATH, SFX_ROBOT_DEATH
             };

    int m_nSFX[22];

    // Copies of stuff that will be re-used
    enum MODELS { MODEL_ENEMY_SWORD, MODEL_ENEMY_SWORD_BOSS, MODEL_ENEMY_SWORD_FAST,
                  MODEL_ENEMY_PISTOL, MODEL_ENEMY_PISTOL_BOSS,
                  MODEL_BULLET, MODEL_ENEMY_BULLET
                };
    SceneNode* m_pModels[7];

    int m_nKeyboardPlayer;
    int m_nJoystickPlayer[2];

    // JOYSTICK!
    // Joystick * m_pJoystick[2];
    // void handleJoystick(Joystick* joy, PC * pc);

    // SHIELDS!
    SceneNode* m_pShield[2];

    // Pause
    bool m_bRunning;

    // Total number of players
    int m_nPlayers;

    // Total number of lives
    int m_nLives;

    int m_nScore;

    // Stat tracking
    int m_frames;
    int m_movements;


};
#endif
