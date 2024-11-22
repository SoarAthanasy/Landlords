#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include "user.h"
#include <QRandomGenerator>
#include "cards.h"
#include "player.h"
#include <QTimer>

struct BetRecord {
    BetRecord() { reset(); }
    void reset() {
        _player = nullptr;  _bet = 0;  _times = 0;
    }
    Player* _player; // 下注玩家
    int _bet;        // 下注的分数
    int _times;      // 第几次叫地主
};

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
        Winning,                 // 赢了
    };

    // 初始化------------------------------------------------
    explicit GameControl(QObject *parent = nullptr);
    void playerInit();   // 玩家初始化
    void initAllCards(); // 初始化扑克牌

    // 得到玩家的实例化对象------------------------------------
    Robot* getLeftRobot();
    Robot* getRightRobot();
    User* getUser();

    // 游戏流程----------------------------------------------
    // 当前玩家
    void setCurrPlayer(Player* player);
    Player* getCurrPlayer();
    // 出牌玩家和打出的牌
    Player* getPendPlayer(); // 出牌玩家
    Cards getPendCards();    // 打出的牌

    Card takeOneCard();      // 每次发一张牌
    Cards getSurplusCards(); // 得到最后3张底牌

    void startLordCard();            // 准备叫地主
    int getPlayerMaxBet();           // 得到玩家下注的最高分数
    void becomeLord(Player* player); // 成为地主

    void resetCardData();    // 重置卡牌数据
    void clearPlayerScore(); // 清空所有玩家的得分

    // 槽函数------------------------------------------------
    void onGrabBet(Player* player, int bet); // 处理叫地主
    // 处理出牌
signals:
    // 通知主窗口玩家的状态发生了变化
    // 因为机器人玩家需要知道自己的状态，才能知道自己应该执行哪段代码
    void playerStatusChanged(Player* player, PlayerStatus status);
    // 通知主窗口, 玩家player已经下注bet分抢地主了, ∴要更新提示信息了
    void notifyGrabLordBet(Player* player, int bet, bool flag);
    void gameStatusChanged(GameStatus status); // 通知主窗口, 游戏状态发生了变化
private:
    Cards _allCards;     // 整副牌
    // 1. 3名玩家
    Robot* _robotLeft;   // 用户左边的机器人
    Robot* _robotRight;  // 用户右边的机器人
    User* _user;         // 用户玩家
    // 2. 游戏流程
    Player* _currPlayer; // 当前玩家
    Player* _pendPlayer; // 待处理牌的所属玩家
    Cards _pendCards;    // 待处理的牌
    // 3. 对局信息
    BetRecord _betRecord; // 记录[下注分数最高的玩家]的下注信息
};

#endif // GAMECONTROL_H
