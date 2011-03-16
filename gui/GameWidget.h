#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QGLWidget>
 
class GameWidget : public QGLWidget {
Q_OBJECT
 
public:
    GameWidget(QWidget * parent = 0);
    ~GameWidget();
 
public slots:
    void showGame();
 
};

#endif
