#include "MainWindow.h"

#include "BoxFactory.h"

#include "QVBoxLayout"
#include "QWidget"

MainWindow::MainWindow(QWidget *parent) {
    // Box Factory Plugin
    Box::Game::BoxFactory box;

    // Making a central Widget for our Window
    QWidget * centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Building our widgets
    _helloWidget = new HelloWidget(this);
    _button = new QPushButton(this);
    _button->setText(box.getTitle().c_str());

    _gameWidget = new GameWidget(this);

    connect(_button, SIGNAL(clicked()), _helloWidget, SLOT(showNewLetter()));

    // Setting the content and disposition of our central Widget
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(_helloWidget);
    layout->addWidget(_button);
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
        sendKeyToGame(GameWidget::UP, true);
        break;
    case 65:
        sendKeyToGame(GameWidget::LEFT, true);
        break;
    case 83:
        sendKeyToGame(GameWidget::DOWN, true);
        break;
    case 68:
        sendKeyToGame(GameWidget::RIGHT, true);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
    case 87:
        sendKeyToGame(GameWidget::UP, false);
        break;
    case 65:
        sendKeyToGame(GameWidget::LEFT, false);
        break;
    case 83:
        sendKeyToGame(GameWidget::DOWN, false);
        break;
    case 68:
        sendKeyToGame(GameWidget::RIGHT, false);
        break;
    }
}

bool MainWindow::sendKeyToGame(GameWidget::GameKey key, bool pressed)
{
    return _gameWidget->handleKeyEvent(key, pressed);
}
