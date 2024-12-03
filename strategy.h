#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "cards.h"
#include "playhand.h"
#include <QMap>
#include <QDebug>

class Strategy {
public:
    Strategy();
    Strategy(Player* player, const Cards& cards);

    Cards makeStrategy();                 // 1. 制定出牌策略
    Cards firstPlay();                    // 2. 第一个出牌firstPlay
    Cards getGreaterCards(PlayHand type); // 3. 得到比指定牌型大的牌
    bool whetherToBeat(Cards& cards);     // 4. 能大过指定的牌时，判断是出牌还是放行: 返回true->出牌, 返回false->放行

    // 获得_cards中符合要求的牌
    // 5. 在_cards中找出count张点数为point的牌：若找到则返回这count张牌，若找不到则返回空对象
    Cards findSamePointCards(Card::CardPoint point, int count);
    // 6. 找出_cards中所有[点数相同且数量为count]的组合牌，返回这些组合牌组成的数组
    QVector<Cards> findCardsByCount(int count);
    // 7. 获取_cards中点数位于[begin, end]中的所有牌
    Cards getRangeCards(Card::CardPoint begin, Card::CardPoint end);
    // 8. 找出_cards中所有与[所出牌hand的牌型_type]相同的组合牌，并通过beat指定要找的组合牌是否要大过指定的所出牌hand
    QVector<Cards> findCardType(PlayHand hand, bool beat);
    // 9. 从指定的Cards对象中挑选出满足条件的顺子
    void pickSeqSingles(QVector<QVector<Cards>>& allSeqRecord, QVector<Cards>& seqSingle, Cards& cards);
    // 10. 从[顺子集合的集合]中筛选出最优的顺子集合
    QVector<Cards> pickOptimalSeqSingle();
private: // 私有成员函数
    using function = Cards (Strategy::*)(Card::CardPoint point);
    struct CardInfo {
        Card::CardPoint _begin;
        Card::CardPoint _end;
        int _extra;  // (顺子/连对)的长度
        bool _beat;
        int _number; // 指定点数的牌的所需数量: 连对-2，顺子-1
        int _base;   // 最基础的(顺子/连对)的长度: 连对-3，顺子-5
        function _getSeq;
    };

    // 获得_cards中所有[点数 ≥ begin ∩ 同点数牌的数量 == number]的组合牌组成的数组
    QVector<Cards> getCards(Card::CardPoint begin, int number);
    // 获取_cards中所有可能的 [ 点数 ≥ begin ] 的 ( 三带一 / 三带二 ) 形式的组合牌
    QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type);
    // 获取_cards中所有可能的 [ 点数 ≥ begin ] 的飞机形式组合牌组成的数组
    QVector<Cards> getPlane(Card::CardPoint begin);
    // 获取_cards中所有可能的 [ 点数 ≥ begin ] 的 ( 飞机带两单 / 飞机带两对 ) 形式组合牌组成的数组
    QVector<Cards> getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type);
    // 获取_cards中所有可能的满足info中各变量要求的(连对/顺子)形式组合牌组成的数组
    QVector<Cards> getSeqPairOrSeqSingle(CardInfo& info);
    Cards getBaseSeqPair(Card::CardPoint point);
    Cards getBaseSeqSingle(Card::CardPoint point);
    // 获取_cards中所有可能的 [ 点数 ≥ begin ] 的炸弹形式组合牌
    QVector<Cards> getBomb(Card::CardPoint begin);
private: // 成员变量
    Player* _player;
    Cards _cards;
};

#endif // STRATEGY_H
