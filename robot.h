#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include "player.h"

class Robot : public Player { // 机器人玩家类
    Q_OBJECT
public:
    using Player::Player; // 可使用Player中所有的构造函数
    explicit Robot(QObject *parent = nullptr);
    void prepareCallLord() override; // 机器人玩家准备抢地主
    void preparePlayHand() override; // 机器人玩家准备出牌
    void thinkCallLord() override;   // 机器人玩家思考抢地主
    void thinkPlayHand() override;   // 机器人玩家思考出牌
};

#endif // ROBOT_H
