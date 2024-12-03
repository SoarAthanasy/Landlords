#ifndef ROBOTPLAYHAND_H
#define ROBOTPLAYHAND_H

#include <QThread>
#include "player.h"

class RobotPlayHand : public QThread {
    Q_OBJECT
public:
    explicit RobotPlayHand(Player* player, QObject *parent = nullptr);
signals:
protected:
    void run() override;
private:
    Player* _player; // 所属玩家
};

#endif // ROBOTPLAYHAND_H
