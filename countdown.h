#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QWidget>
#include <QTimer>

class CountDown : public QWidget {
    Q_OBJECT
public:
    explicit CountDown(QWidget *parent = nullptr);
    void showCountDown(); // 显示倒计时闹钟窗口
    void stopCountDown(); // 停止倒计时, 并隐藏闹钟窗口
signals:
    void notMuchTime(); // 没有太多时间了，∴要播放提示音
    void timeout();      // 计时结束了
protected:
    void paintEvent(QPaintEvent* pe); // 窗口重绘事件
private:
    QPixmap _clock;  // 闹钟的图片
    QPixmap _number; // 数字的图片
    QTimer* _timer;  // 计时器
    int _count;      // 倒计时的总时长
};

#endif // COUNTDOWN_H
