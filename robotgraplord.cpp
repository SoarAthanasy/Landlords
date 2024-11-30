#include "robotgraplord.h"

RobotGrapLord::RobotGrapLord(Player* player, QObject *parent) : QThread{parent} {
    _player = player;
}

void RobotGrapLord::run() {
    sleep(1); // 模拟思考2s
    _player->thinkCallLord();
}
