#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>

class MyButton : public QPushButton { // 自定义按钮类
    Q_OBJECT
public:
    explicit MyButton(QWidget* parent = nullptr);
    // 设置图片路径，并初始化_pixmap
    void setImage(QString normal, QString hover, QString pressed);
protected:
    void mousePressEvent(QMouseEvent* me);   // 鼠标按下
    void mouseReleaseEvent(QMouseEvent* me); // 鼠标释放
    void enterEvent(QEnterEvent* e);         // 鼠标进入
    void leaveEvent(QEvent* e);              // 鼠标离开
    void paintEvent(QPaintEvent* e);         // 绘图
signals:
private:
    QString _normal;  // 普通状态对应图片的路径
    QString _hover;   // 悬停状态对应图片的路径
    QString _pressed; // 按下状态对应图片的路径
    QPixmap _pixmap;  // 按钮应显示的图片
};

#endif // MYBUTTON_H
