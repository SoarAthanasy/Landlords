#include "user.h"

User::User(QObject *parent): Player{parent} {
    _type = Player::User;
}

void User::prepareCallLord() {

}

void User::preparePlayHand() {
    emit startCountDown(); // 开始出牌倒计时
}
