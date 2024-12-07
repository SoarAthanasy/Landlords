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

void AnimationWindow::showSequence(SeqType type) {
    QString name = (type == Pair) ? ":/images/liandui.png" : ":/images/shunzi.png";
    _image.load(name);
    update();
    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::showJokerBomb() {
    _index = 0; // 构成动画的多组图片的索引
    QTimer* timer = new QTimer(this); // 动画所需定时器
    connect(timer, &QTimer::timeout, this, [=](){
        _index++; // 加载下一张图片
        if(_index > 8) { // 动画播放完毕
            timer->stop();
            timer->deleteLater();
            _index = 8;
            hide();
        }
        QString name = QString(":/images/joker_bomb_%1.png").arg(_index);
        _image.load(name);
        update();
    });
    timer->start(60);
}

void AnimationWindow::showBomb() { // 显示炸弹动画
    _index = 0; // 构成动画的多组图片的索引
    QTimer* timer = new QTimer(this); // 动画所需定时器
    connect(timer, &QTimer::timeout, this, [=](){
        _index++; // 加载下一张图片
        if(_index > 12) { // 动画播放完毕
            timer->stop();
            timer->deleteLater();
            _index = 12;
            hide();
        }
        QString name = QString(":/images/bomb_%1.png").arg(_index);
        _image.load(name);
        update();
    });
    timer->start(60);
}

void AnimationWindow::showPlane() {
    _x = width();
    _image.load(":/images/plane_1.png"); // 加载飞机动画的第一张图片
    setFixedHeight(_image.height());     // 固定[动画窗口的高度]为[飞机图片的高度]
    update();
    // 将动画窗口分为5份, 每份的宽度为step，每份中所用飞机图片相同，飞机每次移动5个像素
    int step = width() / 5;    // 每份区域的宽度
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        static int dist = 0;   // 记录飞机移动的距离
        static int timers = 0; // 记录飞机图片的编号
        dist += 5;
        if(dist >= step) {
            dist = 0; // 每走完一份区域, 就要清空dist
            timers++;
            QString name = QString(":/images/plane_%1.png").arg(timers % 5 + 1);
            _image.load(name); // 加载此时所需的飞机图片
        }
        if(_x <= -110) { // 飞机已经飞出动画窗口: 可以停止定时器了
            timer->stop();
            timer->deleteLater();
            dist = timers = 0;
            hide();
        }
        _x -= 5;
        update();
    });
    timer->start(15); // 启动定时器
}

void AnimationWindow::paintEvent(QPaintEvent *pe) {
    Q_UNUSED(pe);
    QPainter p(this);
    p.drawPixmap(_x, 0, _image.width(), _image.height(), _image);
}
