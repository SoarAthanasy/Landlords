#include "cards.h"

Cards::Cards() {}

void Cards::add(const Card& card) {
    _cards.insert(card);
}
void Cards::add(const Cards& cards) {
    _cards.unite(cards._cards); // 取并集
}

Cards& Cards::operator<<(const Card& card) {
    add(card);
    return *this;
}
Cards& Cards::operator<<(const Cards& cards) {
    add(cards);
    return *this;
}

void Cards::remove(Card& card) {
    _cards.remove(card);
}
void Cards::remove(Cards& cards) {
    _cards.subtract(cards._cards);
}

int Cards::cardCount() { return _cards.count(); }

bool Cards::isEmpty() { return _cards.isEmpty(); }

void Cards::clear() { _cards.clear(); }

Card::CardPoint Cards::maxPoint() {
    Card::CardPoint max = Card::Card_Begin;
    if(!_cards.isEmpty()) {
        for(auto it = _cards.begin(); it != _cards.end(); ++it) {
            if(it->point() > max) {
                max = it->point();
            }
        }
    }
    return max;
}

Card::CardPoint Cards::minPoint() {
    Card::CardPoint min = Card::Card_End;
    if(!_cards.isEmpty()) {
        for(auto it = _cards.begin(); it != _cards.end(); ++it) {
            if(it->point() < min) {
                min = it->point();
            }
        }
    }
    return min;
}

int Cards::pointCount(Card::CardPoint point) {
    int count = 0;
    for(auto it = _cards.begin(); it != _cards.end(); ++it) {
        if(it->point() == point) {
            count++;
        }
    }
    return count;
}

bool Cards::contains(const Card &card) const {
    return _cards.contains(card);
}

bool Cards::contains(const Cards &cards) const {
    return _cards.contains(cards._cards);
}

Card Cards::takeRandomCard() {
    int num = QRandomGenerator::global()->bounded(_cards.size()); // 生成一个随机数
    QSet<Card>::const_iterator cit = _cards.constBegin();
    for(int i = 0; i < num; ++i, ++cit);
    Card card = *cit;
    _cards.erase(cit);
    return card;
}

CardList Cards::toCardList(SortType type) {
    CardList list;
    for(auto it = _cards.begin(); it != _cards.end(); ++it) {
        list << *it;
    }
    if(type == Asc) { // 排升序
        // ∵lessSort是对两个Card对象进行比较, ∴建议写到Card.h中
        std::sort(list.begin(), list.end(), lessSort)
    }
    else if(type == Desc) { // 排降序
    }
}











