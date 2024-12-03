#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>

class AnimationWindow : public QWidget { // 特效动画窗口
    Q_OBJECT
public:
    explicit AnimationWindow(QWidget *parent = nullptr);
    void showBetScore(int bet); // 显示下注分数
signals:
protected:
    void paintEvent(QPaintEvent* pe);
private:
    QPixmap _image;
};

#endif // ANIMATIONWINDOW_H
