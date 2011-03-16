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
