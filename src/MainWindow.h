#include <QtGui/QMainWindow>
#include <QPushButton>
#include "HelloWidget.h"
 
class MainWindow : public QMainWindow {
Q_OBJECT
 
public:
    MainWindow(QWidget *parent = 0);
    //~MainWindow();
 
protected:
    HelloWidget* _helloWidget;
    QPushButton* _button;

};
