#ifndef CARDS_H
#define CARDS_H

#include <QSet>
#include "card.h"
#include <QRandomGenerator>

// Cards可用于存储每个玩家的牌, 也可用于存储整副扑克牌
class Cards { // 多张卡牌类
public:
    Cards();
    // 添加扑克牌
    void add(const Card& card);   // 添加1张
    void add(const Cards& cards); // 添加多张
    // 一次性插入多个数据
    Cards& operator<<(const Card& card);
    Cards& operator<<(const Cards& cards);
    // 删除扑克牌
    void remove(Card& card);   // 删除1张
    void remove(Cards& cards); // 删除多张

    int cardCount(); // 扑克牌的数量
    bool isEmpty();  // 判断是否为空
    void clear();    // 清空

    Card::CardPoint maxPoint(); // 最大点数
    Card::CardPoint minPoint(); // 最小点数
    int pointCount(Card::CardPoint point); // 指定点数的牌的数量

    bool contains(const Card& card) const;   // 判断某张牌是否在集合中
    bool contains(const Cards& cards) const; // 判断多张牌是否在集合中

    Card takeRandomCard(); // 随机取出一张扑克牌

    enum SortType { Asc, Desc, NoSort };
    CardList toCardList(SortType type); // 无序QSet -> 有序的QVector, 可指定(升序/降序)
private:
    QSet<Card> _cards;
};

#endif // CARDS_H



