#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include "Game.h"

#include <QSize>
#include <QTimer>
#include <QtOpenGL/QGLWidget>
 

class GameWidget : public QGLWidget {
Q_OBJECT
 
public:
    GameWidget(QWidget * parent = 0);
    virtual ~GameWidget();
 
    enum GameKey { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
                   SHOOT_UP, SHOOT_DOWN, SHOOT_LEFT, SHOOT_RIGHT };
    bool handleKeyEvent(GameKey key, bool pressed);

public slots:
    void tick();
    void paint();
    void stats();
 
protected:

    // QGLWidget implementation /////////////
     
    // Set up the rendering context, define display lists etc.:
    void initializeGL();
    
    // setup viewport, projection etc.:
    void resizeGL(int w, int h);

    // draw the scene:
    void paintGL();

    // End QGLWidget Implementation /////////
    
    // Used by Qt for sizing.
    QSize minimumSizeHint() const { return QSize(640, 480); }
    QSize maximumSizeHint() const { return QSize(640, 480); }
    QSize sizeHint() const { return QSize(640, 480); }

private:
    
    Game m_game;
    QTimer* m_tickTimer;
    QTimer* m_paintTimer;
    QTimer* m_statsTimer;
};

#endif
