#include "mybutton.h"

MyButton::MyButton(QWidget* parent): QPushButton{parent} {}

void MyButton::setImage(QString normal, QString hover, QString pressed) {
    _normal = normal;   _hover = hover;   _pressed = pressed;
    _pixmap.load(normal);
    update();
}

void MyButton::mousePressEvent(QMouseEvent* me) { // 加载鼠标按下对应的图片
    if(me->button() == Qt::LeftButton) {
        _pixmap.load(_pressed);
        update();
    }
    QPushButton::mousePressEvent(me);
}

void MyButton::mouseReleaseEvent(QMouseEvent *me) {
    if(me->button() == Qt::LeftButton) {
        _pixmap.load(_normal);
        update();
    }
    QPushButton::mouseReleaseEvent(me);
}

void MyButton::enterEvent(QEnterEvent *e) {
    Q_UNUSED(e);
    _pixmap.load(_hover);
    update();
}

void MyButton::leaveEvent(QEvent *e) {
    Q_UNUSED(e);
    _pixmap.load(_normal);
    update();
}

void MyButton::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);
    p.drawPixmap(rect(), _pixmap);
}
