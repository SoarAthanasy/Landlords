#include "animationwindow.h"
#include <QPainter>
#include <QTimer>

AnimationWindow::AnimationWindow(QWidget *parent) : QWidget{parent} {}

void AnimationWindow::showBetScore(int bet) {
    if(bet == 1) {
        _image.load(":/images/score1.png");
    }
    else if(bet == 2) {
        _image.load(":/images/score2.png");
    }
    else if(bet == 3) {
        _image.load(":/images/score3.png");
    }
    update();
    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::paintEvent(QPaintEvent *pe) {
    QPainter p(this);
    p.drawPixmap(0, 0, _image.width(), _image.height(), _image);
}
