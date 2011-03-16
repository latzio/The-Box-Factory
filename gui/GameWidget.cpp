#include "GameWidget.h"

#include <iostream>

GameWidget::GameWidget(QWidget* parent)
    :QGLWidget(parent)
{

}

GameWidget::~GameWidget()
{
}

void GameWidget::showGame()
{
    std::cout << "Whoo!" << std::endl;
}
