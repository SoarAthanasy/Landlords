#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>

class AnimationWindow : public QWidget { // 特效动画窗口
    Q_OBJECT
public:
    enum SeqType { Sequence, Pair };
    explicit AnimationWindow(QWidget *parent = nullptr);
    void showBetScore(int bet);      // 显示下注分数
    void showSequence(SeqType type); // 显示顺子和连对
    void showJokerBomb();            // 显示王炸动画
    void showBomb();                 // 显示炸弹动画
    void showPlane();                // 显示飞机
signals:
protected:
    void paintEvent(QPaintEvent* pe);
private:
    QPixmap _image; // 显示图片
    int _index;     // 显示炸弹所需的图片索引: 用于指明显示哪张图片
    int _x = 0;         // 记录飞机当前水平方向的位置
};

#endif // ANIMATIONWINDOW_H
