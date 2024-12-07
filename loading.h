#ifndef LOADING_H
#define LOADING_H

#include <QWidget>

class Loading : public QWidget {
    Q_OBJECT
public:
    explicit Loading(QWidget *parent = nullptr);
signals:
protected:
    void paintEvent(QPaintEvent* pe);
private:
    QPixmap _backImage; // 背景图片
    QPixmap _progress;  // 进度条
    int _distance = 15; // 进度条的显示宽度
};

#endif // LOADING_H
