#ifndef USER_H
#define USER_H

#include <QObject>
#include "player.h"

class User : public Player { // 非机器人玩家类
    Q_OBJECT
public:
    using Player::Player; // 可使用Player中所有的构造函数
    explicit User(QObject *parent = nullptr);
    void prepareCallLord() override;
    void preparePlayHand() override;
signals:
    void startCountDown(); // 通知主窗口: 开始用户玩家的出牌倒计时
};

#endif // USER_H
