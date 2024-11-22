#ifndef CARD_H
#define CARD_H

#include <QVector>

class Card { // 单张卡牌类
public:
    enum CardSuit { // 花色
        Suit_Begin, // 小王、大王
        Diamond,    // 方块
        Club,       // 梅花
        Heart,      // 红桃
        Spade,      // 黑桃
        Suit_End
    };
    enum CardPoint { // 点数
        Card_Begin,
        Card_3, Card_4, Card_5, Card_6, Card_7, Card_8,
        Card_9, Card_10, Card_J, Card_Q, Card_K, Card_A, Card_2,
        Card_SJ, // 小王
        Card_BJ, // 大王
        Card_End
    };

    Card();
    Card(CardPoint point, CardSuit suit);

    void setPoint(CardPoint point); // 设置点数
    void setSuit(CardSuit suit);    // 设置花色
    CardPoint point() const;        // 获取点数
    CardSuit suit() const;          // 获取花色
private:
    CardPoint _point; // 点数
    CardSuit _suit;   // 花色
};

// 比较(点数+花色)
bool lessSort(const Card& c1, const Card& c2);    // 判断c1是否小于c2
bool greaterSort(const Card& c1, const Card& c2); // 判断c1是否大于c2

bool operator==(const Card& left, const Card& right); // 判断c1是否等于c2
uint qHash(const Card& card); // 重写全局函数qHash: 让每张扑克牌转化成不同的数即可

using CardList = QVector<Card>; // 给该类型起别名

bool operator<(const Card& c1, const Card& c2); // 判断c1是否等于c2

#endif // CARD_H
