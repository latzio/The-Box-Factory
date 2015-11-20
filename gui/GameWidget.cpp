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

static QSize s_widgetSize(800, 600);

GameWidget::GameWidget(QWidget* parent)
:m_game(1)
,m_tickTimer(new QTimer(this))
,m_paintTimer(new QTimer(this))
,m_statsTimer(new QTimer(this))
{
    connect(m_tickTimer, SIGNAL(timeout()), this, SLOT(tick()));
    m_tickTimer->start(16);
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

    m_game.init_gl();

}

void GameWidget::resizeGL(int width, int height)
{
    // setup viewport, projection etc.:
    std::cout << "Resize GL called." << std::endl;

    glViewport(0, 0, width, height);
}

void GameWidget::paintGL()
{

    m_game.walk_gl();

}

QSize GameWidget::minimumSizeHint() const
{
    return s_widgetSize;
}

QSize GameWidget::maximumSizeHint() const
{
    return s_widgetSize;
}

QSize GameWidget::sizeHint() const
{
    return s_widgetSize;
}
