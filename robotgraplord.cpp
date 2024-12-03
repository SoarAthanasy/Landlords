#include "robotgraplord.h"
#include "QDebug"

RobotGrapLord::RobotGrapLord(Player* player, QObject *parent) : QThread{parent} {
    _player = player;
}

void RobotGrapLord::run() {
    qDebug() << _player->getName() << "调用了RobotGrapLord::run()";
    sleep(1); // 模拟思考2s
    _player->thinkCallLord();
    qDebug() << _player->getName() << "执行RobotGrapLord::run()完毕";
}
