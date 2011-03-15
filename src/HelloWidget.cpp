#include "HelloWidget.h"
 
HelloWidget::HelloWidget(QWidget * parent) 
    :QLabel(parent)
    ,_msg("Hello World !")
    ,_pos(0) 
{
}
 
HelloWidget::~HelloWidget() {
}
 
void HelloWidget::showNewLetter() {
    _pos++;
    setText(_msg.left(_pos));
}
