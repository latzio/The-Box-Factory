#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QSize>
#include <QtOpenGL/QGLWidget>
 

class GameWidget : public QGLWidget {
Q_OBJECT
 
public:
    GameWidget(QWidget * parent = 0);
    virtual ~GameWidget();
 
public slots:
    void showGame() const;
 
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
};

#endif
