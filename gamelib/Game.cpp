#include "Game.h"
#include "Object.h"
#include "SceneLua.h"
#include <iostream>
#include <cstdlib>
#include <sstream>

// #include "SoundManager.h"
// #include "joystick/include.h"

#define BULLET_CAP 128 
#define ENEMY_CAP  64

#define LITTLE_PARTICLES 92 
#define MEDIUM_PARTICLES 116
#define BIGGER_PARTICLES 128
#define SPARKS_PARTICLES 156
#define PARTICLES        156

#define MOBS     1
#define MOB_SIZE 1
#define ENEMY_WATERMARK 0

#define LEVEL_DELAY 300
#define LIFE_CAP 10

Game::Game( int nPlayers ) 
  : m_nBulletCounter(0)
  , m_nEnemyCounter(0)
  , m_nKeyboardPlayer(0)
  , m_bRunning( true )
  , m_nPlayers( nPlayers )
  , m_nLives( LIFE_CAP )
  , m_nScore( 0 )
{
    /*
  // Import the level
  m_pLevel = new Level( import_lua( "models/level.lua" ), this );;

  // Set up shield lua files
  m_pShield[ 0 ] = import_lua( "models/shield.lua" );
  m_pShield[ 1 ] = import_lua( "models/shield_minor.lua" );

  if ( nPlayers >= 1 ) {
    PC * pc0 = new PC( import_lua( "models/character1.lua" ), this );
    m_PCs.push_back( pc0 );
    pc0->setHumanControlled( true );
  }

  if ( nPlayers >= 2 ) {
    PC * pc1 = new PC( import_lua( "models/character2.lua" ), this );
    m_PCs.push_back( pc1 );
    pc1->setHumanControlled( true );
  }

  if ( nPlayers >= 3 ) {
    PC * pc2 = new PC( import_lua( "models/character3.lua" ), this );
    m_PCs.push_back( pc2 );
    pc2->setHumanControlled( true );
  }

  if ( nPlayers >= 4 ) {
    PC * pc3 = new PC( import_lua( "models/character4.lua" ), this );
    m_PCs.push_back( pc3 );
    pc3->setHumanControlled( true );
  }

  m_Bullets.reserve( BULLET_CAP );
  m_NPCs.reserve( ENEMY_CAP );

  // Create All bullets
  // for (int i = 0; i < BULLET_CAP; i++ ) {
  //	 Bullet * b = Bullet::CreateBullet( this, (Moveable*)0 );
  //	 b->set_id( i );
 // 	 b->set_joint();
  //	 b->set_dead( true );
	 //m_Bullets.push_back( b );
  //}

  // Copies of stuff that will be re-used
  m_pModels[ MODEL_ENEMY_SWORD ] =       import_lua( "models/enemy_sword.lua" );
  m_pModels[ MODEL_ENEMY_SWORD_BOSS ] =  import_lua( "models/enemy_sword.lua" );
  m_pModels[ MODEL_ENEMY_SWORD_BOSS ]->colour( Colour( .2, .2, 2) );
  m_pModels[ MODEL_ENEMY_SWORD_FAST ] =       import_lua( "models/enemy_sword_fast.lua" );
  m_pModels[ MODEL_ENEMY_PISTOL ] =      import_lua( "models/enemy_pistol.lua" );
  m_pModels[ MODEL_ENEMY_PISTOL_BOSS ] = import_lua( "models/enemy_pistol.lua" );
  m_pModels[ MODEL_ENEMY_PISTOL_BOSS ]->colour( Colour( .4, .4, 0) );
  m_pModels[ MODEL_BULLET ] =            import_lua( "models/bullet.lua" );
  m_pModels[ MODEL_ENEMY_BULLET ] =      import_lua( "models/enemy_bullet.lua");

  // Import a bullet
  // m_pDefaultBullet = Bullet::CreateBullet( this, (Moveable*)0 );
  // Import an enemy
  // m_pDefaultEnemy = new NPC( import_lua( "models/enemy.lua" ), this );

  // Create all enemies
  //for( int i = 0; i < ENEMY_CAP; i++ ) {
  //  NPC * npc = new NPC( import_lua( "models/enemy_pistol.lua" ), this );
  //
  //  npc->set_id( i );
//	  npc->set_dead( true );
//
 //   m_NPCs.push_back( npc );
//
 //   m_EnemyIndex.push_back( i );
  //}

  m_nParticleIndex[0] = 0;
  m_nParticleIndex[1] = LITTLE_PARTICLES;
  m_nParticleIndex[2] = MEDIUM_PARTICLES;
  m_nParticleIndex[3] = BIGGER_PARTICLES;

  // Create all particles
  for( int i = 0; i < PARTICLES; i++ ) {

    Particle * p;

    if ( i < LITTLE_PARTICLES ) {
      p = new Particle( import_lua( "models/chunk_little.lua"), this );
    } else if ( i < MEDIUM_PARTICLES ) {
      p = new Particle( import_lua( "models/chunk_medium.lua"), this );
    } else if ( i < BIGGER_PARTICLES ) {
      p = new Particle( import_lua( "models/chunk_bigger.lua"), this );
    } else if ( i < SPARKS_PARTICLES ) {
      p = new Spark( import_lua( "models/chunk_sparks.lua"), this );
    }

    p->set_id( i );
    p->set_dead( true );

    m_Particles.push_back( p );
  }

  //for (int i = 0; i < 100; i++) {
  //  CreateEnemy();
  //}
  
  // Create AI
  m_AIs.push_back( new AutoAI( AI::UP ) );
  m_AIs.push_back( new AutoAI( AI::LEFT ) );
  m_AIs.push_back( new AutoAI( AI::DOWN ) );
  m_AIs.push_back( new AutoAI( AI::RIGHT ) );

  // Create enemies 
  // for(int i = 0; i < MOBS; i++) {
  //
  //  CreateMob( );
  // }

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
		m_pJoystick[i] = loadJoystick( i );
		m_nJoystickPlayer[i] = (i + 1) % nPlayers;
	}
*/

}

Game::~Game() {

}

void Game::play() {

	//int music = SM.LoadMusic("data/WOM.XM");
    /*
	int music = SM.LoadMusic("data/UNREAL.S3M");

  SM.VolumeMusic( 128 );
	SM.PlayMusic( music );

	m_nSFX[0] = SM.LoadSound("data/gunshot.wav");
	m_nSFX[1] = SM.LoadSound("data/carom.wav");

  for (int i = 0; i < 14; i++) {
    std::stringstream ss;
    ss << "data/ricochet/h_1metal" << i << ".wav";
    m_nSFX[ SFX_CAROM0 + i ] = SM.LoadSound( ss.str() );
  }

  //for (int i = 0; i < 2; i++) {
  //  std::stringstream ss;
  //  ss << "data/hit" << i << ".wav";
  //  std::cout << ss.str() << std::endl;
  //  m_nSFX[ SFX_HIT0 + i ] = SM.LoadSound( ss.str() );
  //}

  m_nSFX[ SFX_HIT0 ] = SM.LoadSound( "data/hit0.wav" );
  m_nSFX[ SFX_HIT1 ] = SM.LoadSound( "data/hit1.wav" );

  m_nSFX[ SFX_HUMAN_DEATH ] = SM.LoadSound( "data/player_death.wav" );
  m_nSFX[ SFX_ROBOT_DEATH ] = SM.LoadSound( "data/robot_death.wav" );
*/
}

// Increment one unit of the time in the game world.
// All characters in motion will move, all bullets will move,
// and collisions will be detected. Returns false if all enemies
// are defeated, or both players are out of lives, true otherwise
bool Game::tick() {

  // Check joystick
  /*
  JoystickEvent* event;
  while ( (event = getJoystickEvent()) ) {
    int joystickId = event->getJoystickId();
    if(joystickId == m_pJoystick[ joystickId ]->getId()){
      m_pJoystick[joystickId]->processEvent(event);
      handleJoystick(m_pJoystick[joystickId], m_PCs[ m_nJoystickPlayer[ joystickId ]]);
    }
  }
  */

  // Now stop if paused
  if (!m_bRunning) {
    return true;
  }

  // Fire AI
  for (	AIList::iterator it = m_AIs.begin(); 
			it != m_AIs.end(); it++) {
  		(*it)->tick();
	}

	// Attempt to move players
	for (	PCList::iterator it = m_PCs.begin(); 
			it != m_PCs.end(); it++) {
		(*it)->tick();
	}

	// Attempt to move enemies
	for (	EnemyList::iterator it = m_NPCs.begin(); 
			it != m_NPCs.end(); it++) {
		  (*it)->tick();
	}
	
  // Attempt to move bullets 
  for (unsigned int i = 0; i < m_Bullets.size(); i++) {
    if ( i < m_Bullets.size() ) {
      (m_Bullets[i])->tick();
    }
  }

	// Attempt to move particles
  for (	ParticleList::iterator it = m_Particles.begin(); 
			it != m_Particles.end(); it++) {

	  if (!(*it)->is_dead()) {
      (*it)->tick();
	  }
  }

  // Create some more mobs if we are low on enemies
  while (m_NPCs.size() < ENEMY_WATERMARK) {   
	 for (int i = 0; i < MOBS; i++) { 
	    CreateMob();
	 }
  }

  // Make sure doors are opened and closed
  for (int i = 0; i < 4; i++ ) {
    (m_AIs[i]->isActive()) ? m_pLevel->openDoor( (Level::Door)i) 
                           : m_pLevel->closeDoor( (Level::Door)i);
  }

  m_pLevel->tick();

  if (!m_nLives) {
    std::cout << "Game over." << std::endl;
    std::cout << "  -> Final score: " << m_nScore << std::endl;
  }

	return m_nLives;	
}

void Game::input( Action action, int nPlayer ) {

  if ((unsigned int)nPlayer > m_PCs.size()) { 
    return;
  }
	
	PC* pc = m_PCs[ nPlayer ];

  if (!pc->isHumanControlled()) {
    return;
  }

	switch( action ) {

		// Positive movement and aiming
		case ACTION_MOVE_LEFT:
			pc->setMoving( NPC::LEFT, true );
			break;
		case ACTION_MOVE_UP:
			pc->setMoving( NPC::UP, true );
			break;
		case ACTION_MOVE_RIGHT:
			pc->setMoving( NPC::RIGHT, true );
			break;
		case ACTION_MOVE_DOWN:
			pc->setMoving( NPC::DOWN, true );
			break;
		case ACTION_SHOOT_LEFT:
			pc->setAiming( NPC::LEFT, true );
			break;
		case ACTION_SHOOT_RIGHT:
			pc->setAiming( NPC::RIGHT, true );
			break;
		case ACTION_SHOOT_UP:
			pc->setAiming( NPC::UP, true );
			break;
		case ACTION_SHOOT_DOWN:
			pc->setAiming( NPC::DOWN, true );
			break;

		// Stops 
		case ACTION_STOP_MOVE_LEFT:
			pc->setMoving( NPC::LEFT, false );
			break;
		case ACTION_STOP_MOVE_UP:
			pc->setMoving( NPC::UP, false );
			break;
		case ACTION_STOP_MOVE_RIGHT:
			pc->setMoving( NPC::RIGHT, false );
			break;
		case ACTION_STOP_MOVE_DOWN:
			pc->setMoving( NPC::DOWN, false );
			break;
		case ACTION_STOP_SHOOT_LEFT:
			pc->setAiming( NPC::LEFT, false );
			break;
		case ACTION_STOP_SHOOT_RIGHT:
			pc->setAiming( NPC::RIGHT, false );
			break;
		case ACTION_STOP_SHOOT_UP:
			pc->setAiming( NPC::UP, false );
			break;
		case ACTION_STOP_SHOOT_DOWN:
			pc->setAiming( NPC::DOWN, false );
			break;

		case ACTION_TOGGLE_PAUSE:
      break;

	}


}

/*
void Game::handleJoystick(Joystick* joy, PC * pc){
  
  if (!pc->isHumanControlled()) {
    return;
  }

  if (joy->getButtonDown( Joystick::START )) {
    togglePause();
  }

	//get analog stick values
	double x, y;
	joy->getDPad( x, y );
	if ( x > 0 ) {
		pc->setMoving( NPC::RIGHT, true );
		pc->setMoving( NPC::LEFT, false );
	} else if ( x < 0 ) {
		pc->setMoving( NPC::LEFT, true );
		pc->setMoving( NPC::RIGHT, false );
	} else {
		pc->setMoving( NPC::LEFT, false );
		pc->setMoving( NPC::RIGHT, false );
	}
	
	if ( y > 0 ) {
		pc->setMoving( NPC::DOWN, true );
		pc->setMoving( NPC::UP, false );
	} else if ( y < 0 ) {
		pc->setMoving( NPC::UP, true );
		pc->setMoving( NPC::DOWN, false );
	} else {
		pc->setMoving( NPC::UP, false );
		pc->setMoving( NPC::DOWN, false );
	}

	pc->setAiming( NPC::DOWN,  joy->getButtonDown( Joystick::B ) ? true : false );
	pc->setAiming( NPC::RIGHT, joy->getButtonDown( Joystick::A ) ? true : false );
	pc->setAiming( NPC::UP,    joy->getButtonDown( Joystick::X ) ? true : false );
	pc->setAiming( NPC::LEFT,  joy->getButtonDown( Joystick::Y ) ? true : false );
		
}
*/
void Game::walk_gl() {

    if (!m_pLevel)
        return;

	// draw the level
	glPushMatrix();
	  m_pLevel->walk_gl();
	glPopMatrix();

    return;

	// DRAW PLAYERS 
   // They require scaling down from the lua file
  	glPushMatrix();
  	glScaled( .2, .2, .2 );
  	glTranslated( 0, 7, 0 );

	for (	PCList::iterator it = m_PCs.begin(); 
			it != m_PCs.end(); it++) {
		(*it)->walk_gl();
	}
  	
	
	// Draw the enemies
	for (	EnemyList::iterator it = m_NPCs.begin(); 
			it != m_NPCs.end(); it++) {
  		(*it)->walk_gl();
	}
	
	// END DRAW PLAYERS AND ENEMIES

	// Draw all projectiles
	for (	BulletList::iterator it = m_Bullets.begin(); 
		it != m_Bullets.end(); it++) {
			(*it)->walk_gl();
	}

	// Attempt to draw particles
  for (	ParticleList::iterator it = m_Particles.begin(); 
			it != m_Particles.end(); it++) {

	  if (!(*it)->is_dead()) {
      (*it)->walk_gl();
	  }
  }
  glPopMatrix();

}

void Game::CreateBullet( const Point3D& origin, double dDegrees, NPC* source ) {
  int nQuantity = 1;

  if (m_Bullets.size() + nQuantity > BULLET_CAP) { 
    return;
  }

  Bullet * b;
  for (int i = 0; i < nQuantity; i++) {

    if (source->isNPC()) {
      b = new Bullet( (m_pModels[ MODEL_ENEMY_BULLET ])->clone(), this, source );
      b->set_velocity( .5 );
    } else {
      b = new Bullet( (m_pModels[ MODEL_BULLET ])->clone(), this, source );
      b->set_velocity( 3 );
    }

    b->set_joint();
    b->set_direction( dDegrees );
    b->set_position( origin - Point3D(0,0,0) );
    b->set_dead( false );

    m_Bullets.push_back( b );

    // Muzzle flash
    CreateParticles( SIZE_SPARKS, 1, origin - Point3D(0,0,0 ) );

  }
}

void Game::DeleteBullet( Bullet * pBullet ) {
	// Find and delete bullet
	for (	BulletList::iterator it = m_Bullets.begin(); 
			it != m_Bullets.end(); it++) {
      
        if (*it == pBullet) {
          delete *it;
          m_Bullets.erase( it );
          return;
        }
	}		 

}

#define LEVEL_MAX_X 80
#define LEVEL_MAX_Z 60

Moveable* Game::DetectCollision( Point3D p, double r, Moveable* pExcluded ) {
  Moveable * m = 0;

  // Check for collision with box
  for( ObjList::iterator it = m_Objs.begin(); it != m_Objs.end(); it++) {
    if (!(*it)->is_dead()) {
      m = (*it)->IsHit( p, r );
      if (m && m != pExcluded) {
        return m;
      }
    }
  }

  // Check for collision with enemy
  for( EnemyList::iterator it = m_NPCs.begin(); it != m_NPCs.end(); it++) {
      m = (*it)->IsHit( p, r );
      if (m && m != pExcluded) {

        return m;
      }
  }
	
  // Check for collision with player
  for( PCList::iterator it = m_PCs.begin(); it != m_PCs.end(); it++) {
      m = (*it)->IsHit( p, r );
      if (m && m != pExcluded) {
        return m;
      }
  }
  return m;
}

AISubscriber * Game::CreateEnemy(int x, int z) {

  //int nIndex = m_EnemyIndex.back();
  //m_EnemyIndex.pop_back();

  //NPC * npc = m_NPCs[nIndex];

//  NPC * npc = new NPC();
  int nRand = rand() / (RAND_MAX / 40);
  NPC * npc;

  if (nRand <= 12) {
	  npc = new PistolNPC( (m_pModels[ MODEL_ENEMY_PISTOL])->clone(), this );
    npc->setHealth( 25 * m_PCs.size() );
  } else if (nRand <= 15) {
	  npc = new PistolNPC( (m_pModels[ MODEL_ENEMY_PISTOL_BOSS ])->clone(), this );
    npc->setHealth( 125 * m_PCs.size() );
    npc->setCooldown( 5 );
  } else if (nRand <= 28) {
	  npc = new NPC( (m_pModels[ MODEL_ENEMY_SWORD ])->clone(), this );
    npc->setHealth( 50 * m_PCs.size() );
  } else if (nRand <= 38) {
	  npc = new NPC( (m_pModels[ MODEL_ENEMY_SWORD_FAST ])->clone(), this );
    npc->setHealth( 10 * m_PCs.size() );
    npc->setThrottle( .5 );
  } else {
	  npc = new NPC( (m_pModels[ MODEL_ENEMY_SWORD_BOSS ])->clone(), this );
    npc->setHealth( 200 * m_PCs.size() );
  }

  npc->set_dead( false );
  npc->set_id( m_NPCs.size() );


  int xDiff = (int)(((double)rand() / (double)RAND_MAX - 0.5) * 40); //+ 70;
  int zDiff = (int)(((double)rand() / (double)RAND_MAX - 0.5) * 30); //+ 70;

  npc->set_position( Vector3D(x + xDiff, 0, z + zDiff) );

  m_NPCs.push_back( npc );

  return npc;

}

void Game::DamageEnemy( NPC * pNPC, int nDamage, const Point3D& p3d ) {
 
  Vector3D v = p3d - Point3D(0,0,0);
  CreateParticles( SIZE_LITTLE, 2, v );
  CreateParticles( SIZE_SPARKS, 3, v );

	NPC * npc = pNPC;

  if ( npc->damage( nDamage) ) {

    if (npc->isNPC()) {

      // If dead
      npc->set_dead( true );
      AI* ai = npc->getAI();
      ai->remove( npc );

      // Delete ai if not auto
      if (!ai->isAuto() && !ai->isActive()) {
        DeleteAI( ai );
      }
         
     for (EnemyList::iterator it = m_NPCs.begin();
        it != m_NPCs.end(); it++) {
       if (*it == pNPC) {
         m_NPCs.erase( it );
         break;
       }
     }
    
    if (npc->getType() == Moveable::TYPE_NPC) { 
      CreateParticles( SIZE_BIGGER, 1, v );
    }

     delete npc; 
    
    // Enemy is destroyed, show explosion effects
    CreateParticles( SIZE_MEDIUM, 2, v );

    // SM.PlaySound( m_nSFX[ SFX_ROBOT_DEATH ] );

    // Score
    m_nScore++;

    if (!(m_nScore % 10)) {
      std::cout << m_nScore << " enemies defeated." << std::endl;
    }

  } else {
    // The player was killed
    std::cout << "Player killed. " << --m_nLives << " remaining." << std::endl;
    PC * pc = (PC*)npc;

    pc->die();

    // SM.PlaySound( m_nSFX[ SFX_HUMAN_DEATH ] );
  }


   }   
}

void Game::CreateParticles( ParticleSize eSize, int nQuantity, const Vector3D& v ) {
  
  for (int i = 0; i < nQuantity; i++) {
    Particle * p = m_Particles[ m_nParticleIndex[eSize] ];

    switch(eSize) {
      case SIZE_LITTLE:
        m_nParticleIndex[ eSize ] = (m_nParticleIndex[ eSize ] + 1) % LITTLE_PARTICLES;
        break;
      case SIZE_MEDIUM:
        m_nParticleIndex[ eSize ] = (m_nParticleIndex[ eSize ] + 1 < MEDIUM_PARTICLES) ?
                                    m_nParticleIndex[ eSize ] + 1: LITTLE_PARTICLES;
        break;
      case SIZE_BIGGER:
        m_nParticleIndex[ eSize ] = (m_nParticleIndex[ eSize ] + 1 < BIGGER_PARTICLES) ?
                                    m_nParticleIndex[ eSize ] + 1: MEDIUM_PARTICLES;
        break;
      case SIZE_SPARKS:
        m_nParticleIndex[ eSize ] = (m_nParticleIndex[ eSize ] + 1 < PARTICLES) ?
                                    m_nParticleIndex[ eSize ] + 1: BIGGER_PARTICLES;
        break;
    }

    p->set_position( v );
    p->reset();
    p->set_dead( false );
  }
}

void Game::CreateMob( ) {

  AI * ai;
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

  x += (int)(((double)rand() / (double)RAND_MAX -0.5) * 5);
  z += (int)(((double)rand() / (double)RAND_MAX -0.5) * 5);

  // Create AI to auto move

  for (int i = 0; i < MOB_SIZE; i++ ){
    ai->addSubscriber( CreateEnemy(x, z) );
  }

  //m_AIs.push_back( ai );
  // ai->addTarget( m_PCs[0] );

}

void Game::CreateAI( NPC * npc ) {

  AI * ai = new AI();
  ai->addSubscriber( npc );

  ai->addTarget( m_PCs[ rand() / (RAND_MAX / m_PCs.size())] );

  m_AIs.push_back( ai );

}

void Game::DeleteAI( AI * ai ) {

  for( AIList::iterator it = m_AIs.begin(); 
       it != m_AIs.end(); it++ ) {
    if (*it == ai) {
      delete *it;
      m_AIs.erase( it );
      return;
    }
  }
}

void Game::PlaySFX( MoveableSubscriber::SFX id ) {
	switch( id ) {

		case MoveableSubscriber::SFX_BULLET:
		case MoveableSubscriber::SFX_BLAST:
		case MoveableSubscriber::SFX_BOMB:
  			// SM.PlaySound(m_nSFX[0] );
			break;

		case MoveableSubscriber::SFX_CAROM:
    {
        // int nRand = ((double)(rand() / (double)RAND_MAX) * 16) + SFX_CAROM0;
        // SM.PlaySound( m_nSFX[ nRand ] );
        break;
    }
		case MoveableSubscriber::SFX_HUMAN_DEATH:
    {
        // SM.PlaySound( m_nSFX[ SFX_HUMAN_DEATH ] );
        break;
    }
		
    case MoveableSubscriber::SFX_ROBOT_HIT:
    case MoveableSubscriber::SFX_ROBOT_DEATH:
        // SM.PlaySound( SFX_ROBOT_DEATH );
        break;

	}

}

void Game::CreateObstacle( Moveable* pObstacle ) { 
  pObstacle->set_id ( m_Objs.size() );
  m_Objs.push_back( pObstacle ); 
  pObstacle->set_dead( false );
}

Shield* Game::RequestShield( MoveableSubscriber::ShieldType s ) {

  Shield * shield = 0;

  switch(s) {
    case MoveableSubscriber::MAJOR:
      shield = new Shield( m_pShield[0]->clone(), this );
      shield->setTTL( 240 );
      shield->setMajor( true );
      break;
    case MoveableSubscriber::MINOR:
      shield = new Shield( m_pShield[1]->clone(), this );
      shield->setTTL( 120 );
      shield->setMajor( false );
      break;
  }

  return shield;

}
