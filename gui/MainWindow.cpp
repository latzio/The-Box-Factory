#include "MainWindow.h"

#include "BoxFactory.h"

#include "QVBoxLayout"
#include "QWidget"

MainWindow::MainWindow(QWidget *parent)
    : _gameWidget(new GameWidget(this))
{
    // Box Factory Plugin
    Box::Game::BoxFactory box;

    // Making a central Widget for our Window
    QWidget * centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Setting the content and disposition of our central Widget
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_gameWidget);
    centralWidget->setLayout(layout);
}

MainWindow::~MainWindow() {
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
    case 87:
        sendKeyToGame(GameWidget::MOVE_UP, true);
        break;
    case 65:
        sendKeyToGame(GameWidget::MOVE_LEFT, true);
        break;
    case 83:
        sendKeyToGame(GameWidget::MOVE_DOWN, true);
        break;
    case 68:
        sendKeyToGame(GameWidget::MOVE_RIGHT, true);
        break;
    case 'I':
        sendKeyToGame(GameWidget::SHOOT_UP, true);
        break;
    case 'J':
        sendKeyToGame(GameWidget::SHOOT_LEFT, true);
        break;
    case 'K':
        sendKeyToGame(GameWidget::SHOOT_DOWN, true);
        break;
    case 'L':
        sendKeyToGame(GameWidget::SHOOT_RIGHT, true);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
    case 87:
        sendKeyToGame(GameWidget::MOVE_UP, false);
        break;
    case 65:
        sendKeyToGame(GameWidget::MOVE_LEFT, false);
        break;
    case 83:
        sendKeyToGame(GameWidget::MOVE_DOWN, false);
        break;
    case 68:
        sendKeyToGame(GameWidget::MOVE_RIGHT, false);
        break;
    case 'I':
        sendKeyToGame(GameWidget::SHOOT_UP, false);
        break;
    case 'J':
        sendKeyToGame(GameWidget::SHOOT_LEFT, false);
        break;
    case 'K':
        sendKeyToGame(GameWidget::SHOOT_DOWN, false);
        break;
    case 'L':
        sendKeyToGame(GameWidget::SHOOT_RIGHT, false);
        break;
    }
}

bool MainWindow::sendKeyToGame(GameWidget::GameKey key, bool pressed)
{
    return _gameWidget->handleKeyEvent(key, pressed);
}
