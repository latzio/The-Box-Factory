#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "HelloWidget.h"
#include "GameWidget.h"

#include <QKeyEvent>
#include <QtGui/QMainWindow>
#include <QPushButton>
 
class MainWindow : public QMainWindow {
Q_OBJECT
 
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
 
protected:

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent*event);

    bool sendKeyToGame(GameWidget::GameKey key, bool pressed);

    HelloWidget* _helloWidget;
    QPushButton* _button;
    GameWidget*  _gameWidget;

};

#endif
