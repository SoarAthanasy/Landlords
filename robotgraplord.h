#ifndef ROBOTGRAPLORD_H
#define ROBOTGRAPLORD_H

#include <QThread>
#include "player.h"

class RobotGrapLord : public QThread {
    Q_OBJECT
public:
    explicit RobotGrapLord(Player* player, QObject *parent = nullptr);
protected:
    void run();
private:
    Player* _player;
};

#endif // ROBOTGRAPLORD_H
