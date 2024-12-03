#include "robot.h"
#include "strategy.h"
#include "robotgraplord.h"
#include "robotplayhand.h"
#include "QDebug"

Robot::Robot(QObject *parent): Player{parent} {
    _type = Player::Robot;
}

void Robot::prepareCallLord() {
    RobotGrapLord* subThread = new RobotGrapLord(this);
    subThread->start();
}

void Robot::preparePlayHand() {
    RobotPlayHand* subThread = new RobotPlayHand(this);
    subThread->start();
}

void Robot::thinkCallLord() {
    /* 基于手牌计算权重:
     * 大小王: 6
     * 顺子/炸弹: 5
     * 三张点数相同的牌: 4
     * 2的权重: 3
     * 对子: 1
    */
    int weight = 0; // 记录手牌的权重
    Strategy strategy(this, _cards); // 创建基于 [ 当前机器人玩家的手牌 ] 的策略类
    // 1. 记录手牌中大小王的权重
    weight += strategy.getRangeCards(Card::Card_SJ, Card::Card_End).cardCount() * 6;

    // 2. 记录手牌中顺子的权重
    // pickOptimalSeqSingle()有问题!!!!!
    QVector<Cards> optSeqSingle = strategy.pickOptimalSeqSingle(); // 获取手牌的[所有可能的顺子集合]中最优的顺子集合
    weight += optSeqSingle.size() * 5;

    // 3. 记录手牌中炸弹的权重
    QVector<Cards> bombs = strategy.findCardsByCount(4);
    weight += bombs.size() * 5;

    // 4. 记录手牌中[点数为2的牌]的权重
    weight += _cards.pointCount(Card::Card_2) * 3;

    // 5. 记录手牌中[三张同点数牌]的权重
    Cards tmp = _cards;
    tmp.remove(optSeqSingle); // 去除已计算的顺子
    tmp.remove(bombs);        // 去除已计算的炸弹
    Cards card2 = strategy.getRangeCards(Card::Card_2, Card::Card_SJ);
    tmp.remove(card2);        // 去除已计算的[点数为2的牌]
    QVector<Cards> triples = Strategy(this, tmp).findCardsByCount(3);
    weight += triples.size() * 4;

    // 6. 记录手牌中[对子]的权重
    tmp.remove(triples);      // 再去除已计算的三张点数相同的牌
    QVector<Cards> pairs = Strategy(this, tmp).findCardsByCount(2);
    weight += pairs.size() * 1;

    // 7. 根据所得权重, 决定下注的分数
    if(weight >= 22) {
        grabLordBet(3); // 权重大于22，则下注3分
    }
    else if(weight < 22 && weight >= 18) {
        grabLordBet(2);
    }
    else if(weight < 18 && weight >= 10) {
        grabLordBet(1);
    }
    else {
        grabLordBet(0);
    }
}

void Robot::thinkPlayHand() {
    Strategy st(this, _cards);       // 基于当前玩家及其手牌创建的策略类对象
    qDebug() << _name << "调用了run() -> thinkPlayHand() -> Strategy::makeStrategy()";
    Cards cards = st.makeStrategy(); // 得到要出的牌
    qDebug() << _name << "调用run() -> thinkPlayHand() -> Strategy::makeStrategy()成功";
    playHand(cards);                 // 当前玩家打出cards
}
