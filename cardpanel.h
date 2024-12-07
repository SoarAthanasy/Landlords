#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include <QPainter>
#include "card.h"
#include "player.h"

class CardPanel : public QWidget { // 单张卡牌窗口类
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);

    void setImage(const QPixmap& front, const QPixmap& back); // 设置卡牌的正面和背面的图片
    QPixmap getImage(); // 获取卡牌的正面图片。为什么不获取背面?——背面都相同, 没有获取价值

    void setFrontSide(bool flag); // 设置是否显示正面
    bool isFrontSide();           // 判断是否显示正面

    void setSelected(bool flag); // 设置窗口是否被选中
    bool isSelected();           // 判断窗口是否被选中

    void setCard(const Card& card); // 设置扑克牌的花色及点数
    Card getCard();                 // 获取扑克牌数据

    void setOwner(Player* player); // 设置卡牌的所有者
    Player* getOwner();            // 获取卡牌的所有者

    void clicked(); // 模拟扑克牌的鼠标点击事件
protected:
    // 通过setImage()只能将图片传入, 但要显示图片还需paintEvent事件处理函数
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);  // 点击卡片窗口: 发送cardSelected()信号
signals:
    void cardSelected(Qt::MouseButton button); // 通知主窗口, 当前卡牌窗口被选中了
private:
    QPixmap _front;   // 正面的图片
    QPixmap _back;    // 背面的图片
    bool _isFront = true;    // 判断卡牌当前是否为正面
    bool _isSelected = false; // 记录卡牌是否被选中
    Card _card;       // 对应的卡牌
    Player* _owner = nullptr;   // 卡牌的所有者
};

#endif // CARDPANEL_H
