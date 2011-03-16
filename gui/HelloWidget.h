#ifndef HELLO_WIDGET_H
#define HELLO_WIDGET_H

#include <QLabel>
#include <QString>
 
class HelloWidget : public QLabel {
Q_OBJECT
 
public:
    HelloWidget(QWidget * parent = 0);
    ~HelloWidget();
 
public slots:
    void showNewLetter();
 
protected:
    QString _msg;
    unsigned short _pos;
};

#endif
