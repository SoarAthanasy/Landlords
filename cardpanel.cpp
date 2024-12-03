#include "cardpanel.h"
#include <QMouseEvent>

CardPanel::CardPanel(QWidget *parent): QWidget{parent} {
    _isFront = true;
}

void CardPanel::setImage(QPixmap& front, QPixmap& back) {
    _front = front;
    _back = back;
    setFixedSize(_front.size()); // 卡牌窗口的大小固定等于正面图片的大小
    update(); // 通知Qt去调用paintEvent
}
QPixmap CardPanel::getImage() { return _front; }

void CardPanel::setFrontSide(bool flag) { _isFront = flag; }
bool CardPanel::isFrontSide() { return _isFront; }

void CardPanel::setSelected(bool flag) { _isSelected = flag; }
bool CardPanel::isSelected() { return _isSelected; }

void CardPanel::setCard(const Card &card) { _card = card; }
Card CardPanel::getCard() { return _card; }

void CardPanel::setOwner(Player *player) { _owner = player; }
Player *CardPanel::getOwner() { return _owner; }

void CardPanel::clicked() {
    emit cardSelected(Qt::LeftButton);
}

void CardPanel::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter p(this); // 画图需要使用画图类, 并需要指定绘画设备类
    if(_isFront) { p.drawPixmap(rect(), _front); }
    else { p.drawPixmap(rect(), _back); }
}

void CardPanel::mousePressEvent(QMouseEvent *event) {
    emit cardSelected(event->button());
}
