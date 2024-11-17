#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include "user.h"
#include <QRandomGenerator>
#include "cards.h"
#include "player.h"

class GameControl : public QObject { // 游戏控制类
    Q_OBJECT
public:
    enum GameStatus { // 游戏状态
        Dispatch,     // 发牌状态
        CallingLord,  // 叫地主状态
        PlayingHand   // 出牌状态
    };
    enum PlayerStatus { // 玩家状态
        ThinkingForCallLord, // 考虑叫地主
        ThinkingForPlayHand, // 考虑出牌
        Win,                 // 赢了
        Loss                 // 输了
    };

    explicit GameControl(QObject *parent = nullptr);

    void playerInit(); // 玩家初始化
    // 得到玩家的实例化对象
    Robot* getLeftRobot();
    Robot* getRightRobot();
    User* getUser();
    // 当前玩家
    void setCurrPlayer(Player* player);
    Player* getCurrPlayer();
    // 出牌玩家和打出的牌
    Player* getPendPlayer(); // 出牌玩家
    Cards getPendCards();    // 打出的牌

    void initAllCards();     // 初始化扑克牌
    Card takeOneCard();      // 每次发一张牌
    Cards getSurplusCards(); // 得到最后3张底牌
    void resetCardData();    // 重置卡牌数据

    void startLordCard();            // 准备叫地主
    void becomeLord(Player* player); // 成为地主
    // 处理叫地主
    // 处理出牌

    void clearPlayerScore(); // 清空所有玩家的得分
signals:
private:
    Robot* _robotLeft;   // 用户左边的机器人
    Robot* _robotRight;  // 用户右边的机器人
    User* _user;         // 用户玩家
    Player* _currPlayer; // 当前玩家
    Player* _pendPlayer; // 待处理牌的所属我那家
    Cards _pendCards;    // 待处理的牌
    Cards _allCards;     // 整副牌
};

#endif // GAMECONTROL_H
