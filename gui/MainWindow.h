#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "GameWidget.h"

#include <QKeyEvent>
#include <QtGui/QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

protected:

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent*event);

    bool sendKeyToGame(GameWidget::GameKey key, bool pressed);

    GameWidget*  _gameWidget;

};

#endif
