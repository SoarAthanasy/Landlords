#include "countdown.h"
#include <QPainter>

CountDown::CountDown(QWidget *parent) : QWidget{parent} {
    setFixedSize(70, 70);
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, [=](){
        _count--;
        if(_count > 0 && _count < 10) {
            _clock.load(":/images/clock.png");
            _number = QPixmap(":/images/number.png").copy(_count * (30 + 10), 0, 30, 42).scaled(20, 30);
            if(_count == 5) {
                emit notMuchTime();
            }
        }
        else if(_count <= 0) {
            _clock = QPixmap();
            _number = QPixmap();
            _timer->stop();
            emit timeout();
        }
        update();
    });
}

void CountDown::showCountDown() {
    _count = 15;
    _timer->start(1000); // 设置计时器间隔为1s
}

void CountDown::stopCountDown() {
    _timer->stop();
    _clock = QPixmap();
    _number = QPixmap();
    update();
}

void CountDown::paintEvent(QPaintEvent *pe) {
    QPainter p(this);
    p.drawPixmap(rect(), _clock);
    p.drawPixmap(24, 24, _number.width(), _number.height(), _number);
}
