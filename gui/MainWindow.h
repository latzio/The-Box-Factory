#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "HelloWidget.h"
#include "GameWidget.h"

#include <QtGui/QMainWindow>
#include <QPushButton>
 
class MainWindow : public QMainWindow {
Q_OBJECT
 
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
 
protected:
    HelloWidget* _helloWidget;
    QPushButton* _button;
    GameWidget*  _gameWidget;

};

#endif
