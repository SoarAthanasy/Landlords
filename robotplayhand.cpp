#include "robotplayhand.h"
#include <QDebug>

RobotPlayHand::RobotPlayHand(Player* player, QObject *parent) : QThread{parent} {
    _player = player;
}

void RobotPlayHand::run() {
    qDebug() << _player->getName() << "调用了RobotPlayHand::run()";
    msleep(1000);
    _player->thinkPlayHand();
    qDebug() << _player->getName() << "执行RobotPlayHand::run()成功";
}
