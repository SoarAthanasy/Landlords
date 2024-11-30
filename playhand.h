#ifndef PLAYHAND_H
#define PLAYHAND_H

#include "card.h"
#include "cards.h"
#include <QVector>

class PlayHand {
public:
    enum HandType { // 出牌的(组合or方式)
        Hand_Unknow, // 未知
        Hand_Pass,   // 过

        Hand_Single, // 单
        Hand_Pair,   // 对子
        Hand_Triple,        // 三张
        Hand_Triple_Single, // 三带一
        Hand_Triple_Pair,   // 三带二

        Hand_Plane,            // 飞机, 555_666
        Hand_Plane_Two_Single, // 飞机带单, 555_666_3_4
        Hand_Plane_Two_Pair,   // 飞机带对, 555_666_33_44

        Hand_Seq_Pair,   // 连对, 33_44_55(_66...)
        Hand_Seq_Single, // 顺子, 34567(8...)

        Hand_Bomb,            // 炸弹
        Hand_Bomb_Single,     // 炸弹带一个
        Hand_Bomb_Pair,       // 炸弹带一对
        Hand_Bomb_Two_Single, // 炸弹带两单

        Hand_Bomb_Jokers,           // 王炸
        Hand_Bomb_Jokers_Single,    // 王炸带一个
        Hand_Bomb_Jokers_Pair,      // 王炸带一对
        Hand_Bomb_Jokers_Two_Single // 王炸带两单
    };

    PlayHand();
    // 传递给类一组牌, 通过类分析出牌型, 点数, 以及相关的附属信息(如顺子: 记录牌的数量)
    PlayHand(Cards& cards);
    PlayHand(HandType type, Card::CardPoint point, int extra);

    // 得到组合牌的属性信息
    HandType getHandType();
    Card::CardPoint getCardPoint();
    int getExtra();

    // 比较当前组合牌和其他组合牌的大小
    bool canBeat(const PlayHand& other);
private: // 私有成员函数
    // 对cards中的牌进行分类: (1张/2张/3张/4张)相同点数的有几种
    void classify(Cards& cards);
    void judgeCardType(); // 对牌型进行分类

    // 判断牌的类型
    bool isPass();                // 过
    bool isSingle();              // 单
    bool isPair();                // 对
    bool isTriple();              // 三个相同
    bool isTripleSingle();        // 三带一
    bool isTriplePair();          // 三带二
    bool isPlane();               // 飞机
    bool isPlaneTwoSingle();      // 飞机带两单
    bool isPlaneTwoPair();        // 飞机带两对
    bool isSeqPair();             // 连对
    bool isSeqSingle();           // 顺子
    bool isBomb();                // 炸弹
    bool isBombSingle();          // 炸弹带一单
    bool isBombPair();            // 炸弹带一对
    bool isBombTwoSingle();       // 炸弹带两单
    bool isBombJokers();          // 王炸
    bool isBombJokersSingle();    // 王炸带一单
    bool isBombJokersPair();      // 王炸带一对
    bool isBombJokersTwoSingle(); // 王炸带两单
private: // 成员变量
    HandType _type;         // 所出牌的类型
    Card::CardPoint _point; // 所出牌的点数
    int _extra;             // 所出牌的相关信息: 连对有多少对, 顺子有多少张牌
    QVector<Card::CardPoint> _oneCard;   // 存储: 所出牌中含有的单张牌的点数
    QVector<Card::CardPoint> _twoCard;   // 存储: 所出牌中含有的[点数相同的2张牌]的点数
    QVector<Card::CardPoint> _threeCard; // 存储: 所出牌中含有的[点数相同的3张牌]的点数
    QVector<Card::CardPoint> _fourCard;  // 存储: 所出牌中含有的[点数相同的4张牌]的点数
};

#endif // PLAYHAND_H
