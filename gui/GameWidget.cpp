#include "GameWidget.h"

#include <iostream>
#include <QtGui>

GameWidget::GameWidget(QWidget* parent)
{

}

GameWidget::~GameWidget()
{
}

void GameWidget::showGame() const
{
    std::cout << "Whoo!" << std::endl;
}
     
void GameWidget::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
    std::cout << "Initialize GL called." << std::endl;
    
    glClearColor(0.0, 0.3, 0.1, 0.0);

    //logo = new QtLogo(this, 64);
    //logo->setColor(qtGreen.dark());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

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
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(16.0, 1.0, 0.0, 0.0);
    glRotatef(16.0, 0.0, 1.0, 0.0);
    glRotatef(16.0, 0.0, 0.0, 1.0);

}
