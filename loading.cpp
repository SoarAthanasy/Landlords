#include "loading.h"
#include <QPainter>
#include <QTimer>
#include "gamepanel.h"

Loading::Loading(QWidget *parent) : QWidget{parent} {
    _backImage.load(":/images/loading.png"); // 加载背景图片
    setFixedSize(_backImage.size());         // 固定窗口的大小为背景图片的大小
    setWindowFlags(Qt::FramelessWindowHint | windowFlags()); // 隐藏窗口的边框
    setAttribute(Qt::WA_TranslucentBackground);

    QPixmap pixmap(":/images/progress.png"); // 获取进度条图片
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        _progress = pixmap.copy(0, 0, _distance, pixmap.height()); // 显示宽度为_distance进度条
        update();
        if( _distance >= pixmap.width() ) { // 进度条显示完毕
            timer->stop(); // 停止定时器
            timer->deleteLater();
            GamePanel* panel = new GamePanel;
            panel->show();
            close(); // 关掉游戏加载窗口
        }
        _distance += 5; // 增加显示进度条的宽度
    });
    timer->start(15);
}

void Loading::paintEvent(QPaintEvent *pe) {
    Q_UNUSED(pe);
    QPainter p(this);
    p.drawPixmap(rect(), _backImage);
    p.drawPixmap(62, 417, _progress.width(), _progress.height(), _progress);
}
