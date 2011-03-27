#include "GameWidget.h"

#include "Algebra.h"
#include "Game.h"
#include "Image.h"
#include "SceneLua.h"
#include "Textures.h"

#include <iostream>
#include <QtGui>
#include <GL/gl.h>
#include <GL/glu.h>

GameWidget::GameWidget(QWidget* parent)
:m_game(1)
,m_tickTimer(new QTimer(this))
,m_paintTimer(new QTimer(this))
,m_statsTimer(new QTimer(this))
{
    connect(m_tickTimer, SIGNAL(timeout()), this, SLOT(tick()));
    m_tickTimer->start(20);
//    connect(m_paintTimer, SIGNAL(timeout()), this, SLOT(paint()));
//    m_paintTimer->start(32);
    connect(m_statsTimer, SIGNAL(timeout()), this, SLOT(stats()));
    m_statsTimer->start(10000);
}

GameWidget::~GameWidget()
{
    m_tickTimer->stop();
    delete m_tickTimer;
    m_tickTimer = 0;
    m_paintTimer->stop();
    delete m_paintTimer;
    m_paintTimer = 0;
    m_statsTimer->stop();
    delete m_statsTimer;
    m_statsTimer = 0;
}

bool GameWidget::handleKeyEvent(GameWidget::GameKey key, bool pressed)
{
    Game::Action action = Game::ACTION_MOVE_LEFT;
    switch (key) {
    case MOVE_UP:
        action = Game::ACTION_MOVE_UP;
        break;
    case MOVE_DOWN:
        action = Game::ACTION_MOVE_DOWN;
        break;
    case MOVE_RIGHT:
        action = Game::ACTION_MOVE_RIGHT;
        break;
    case MOVE_LEFT:
        action = Game::ACTION_MOVE_LEFT;
        break;
    case SHOOT_UP:
        action = Game::ACTION_SHOOT_UP;
        break;
    case SHOOT_DOWN:
        action = Game::ACTION_SHOOT_DOWN;
        break;
    case SHOOT_RIGHT:
        action = Game::ACTION_SHOOT_RIGHT;
        break;
    case SHOOT_LEFT:
        action = Game::ACTION_SHOOT_LEFT;
        break;
    }
    if (!pressed)
        action = (Game::Action)((int)action + 1);

    m_game.input(action, 0);
        
    return true;
}

void GameWidget::tick()
{
    m_game.tick();
    paint();
}
     
void GameWidget::paint()
{
    updateGL();
}

void GameWidget::stats()
{
    m_game.dumpStats();
    m_game.clearStats();
}
     
void GameWidget::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
    std::cout << "Initialize GL called." << std::endl;
    
    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    //logo = new QtLogo(this, 64);
    //logo->setColor(qtGreen.dark());

    glEnable(GL_CULL_FACE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_MULTISAMPLE);
    
    //static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // DISPLAY LIST
    SceneNode::setupDL();

    // IMAGE LOADER
    Image * i = new Image();

    // Texture mapping
    glEnable(GL_TEXTURE_2D);

    std::cout << "Textures: " << TEXTURE_COUNT << std::endl;

    for( int c = 1; c < TEXTURE_COUNT; c++ ) {

        // Load into OpenGL
        i->loadPng( Textures::texFolder + Textures::texPaths[ c ] );
        std::cout << "Loading texture from: " << Textures::texFolder + Textures::texPaths[ c ] << std::endl;
        glBindTexture(GL_TEXTURE_2D, Textures::TEX_NO_TEXTURE + c );
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, i->width(), i->height(), 
        0, (i->elements() == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, i->byteData());

    }

    m_game.play();

}

void GameWidget::resizeGL(int width, int height)
{
    // setup viewport, projection etc.:
    std::cout << "Resize GL called." << std::endl;
    
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);

}

void GameWidget::paintGL()
{

    // draw the scene:
    int width = 640; //get_width();
    int height = 480; //get_height();

    // Set up for perspective drawing 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Set up culling!
    // glDisable(GL_CULL_FACE);

    glViewport(0, 0, width, height);
    gluPerspective(40.0, (GLfloat)width/(GLfloat)height, .1, 1000.0);

    // change to model view for drawing
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply user camera changes
    // glTranslated( m_nTrans[0], m_nTrans[1], m_nTrans[2] );

    // Back up the camera
    glTranslated( 0, 0, -40 );
    glRotated( 70, 1, 0, 0 );


    // Apply user rotation
    // glMultMatrixd( m_rotate.transpose().begin() );

    /*/ Draw stuff
    glPushMatrix();
    glScaled( .2, .2, .2 );
    glTranslated( 0, 7, 0 );
    m_objects->walk_gl( picking );
    glPopMatrix();

    glPushMatrix();
    m_scene->walk_gl( picking );
    glPopMatrix();

    */
    // Set up lighting
    glEnable ( GL_LIGHTING ) ;
    glEnable ( GL_COLOR_MATERIAL ) ;

    // Enable normalizing of normals
    glEnable(GL_NORMALIZE);
    // glEnable(GL_RESCALE_NORMAL);
    // glEnable(GL_AUTO_NORMAL);

    // Light 0 // Ambient
    GLfloat light0[] = { .12, .12, .08, 1 };
    glLightfv( GL_LIGHT0, GL_AMBIENT, light0);
    glEnable(GL_LIGHT0);

    GLfloat light[] = { 0.9, 0.9, 0.7, 1 };
    //									{ 0.8, 0.8, 0.6, 1 },
    //									{ 0.8, 0.8, 0.6, 1 },
    //									{ 0.8, 0.8, 0.6, 1 } };

    GLfloat pos[][4]  =  { { -8, 10, -8, 1 },
                              { 7, 10, -7, 1 },
                              { -7, 10, 7, 1 },
                              { 8, 10, 8, 1 }};

    GLfloat dir[] = {0, -1, 0};

    for (int i = 0; i < 4; i++) {
     for (int j = 0; j < 2; j++) {
        glLightfv( GL_LIGHT1 + i, GL_DIFFUSE, light);
        glLightfv( GL_LIGHT1 + i, GL_POSITION, pos[i]);
         glLightfv( GL_LIGHT1 + i, GL_SPOT_DIRECTION, dir);
         glLightf ( GL_LIGHT1 + i, GL_SPOT_CUTOFF, 45.f);
         //glLightf ( GL_LIGHT1 + i, GL_CONSTANT_ATTENUATION, 0.95);
        glEnable(GL_LIGHT1 + i);
     }
    }

    glPushMatrix();
    m_game.walk_gl();
    glPopMatrix();
    
}
