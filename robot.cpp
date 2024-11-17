#include "robot.h"

Robot::Robot(QObject *parent): Player{parent} {
    _type = Player::Robot;
}

void Robot::prepareCallLord() { }

void Robot::preparePlayHand() {

}
