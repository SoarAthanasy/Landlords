#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include "player.h"

class Robot : public Player { // 机器人玩家类
    Q_OBJECT
public:
    using Player::Player; // 可使用Player中所有的构造函数
    explicit Robot(QObject *parent = nullptr);
    void prepareCallLord() override;
    void preparePlayHand() override;
};

#endif // ROBOT_H
