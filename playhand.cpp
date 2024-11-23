#include "playhand.h"

PlayHand::PlayHand() {}

PlayHand::PlayHand(Cards &cards) {
    // 1. 对cards中的牌进行分类: (1张/2张/3张/4张)相同点数的有几种
    classify(cards);
    // 2. 对牌型进行分类
    judgeCardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint point, int extra) {
    _type = type;  _point = point;  _extra = extra;
}

void PlayHand::classify(Cards &cards) {
    CardList list = cards.toCardList();
    // 存储cards中各点数的牌的数量
    int cardRecord[Card::Card_End]; // 这样定义，即可用CardPoint的枚举值作索引
    memset(cardRecord, 0, sizeof(int) * Card::Card_End); // 清零cardRecord
    for(int i = 0; i < list.size(); ++i) {
        Card card = list.at(i);
        cardRecord[card.point()]++;
    }
    _oneCard.clear();
    _twoCard.clear();
    _threeCard.clear();
    _fourCard.clear();
    for(int i = 0; i < Card::Card_End; ++i) {
        if(cardRecord[i] == 1) {
            _oneCard.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i] == 2) {
            _twoCard.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i] == 3) {
            _threeCard.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i] == 4) {
            _fourCard.push_back((Card::CardPoint)i);
        }
    }
}

void PlayHand::judgeCardType() {

}

PlayHand::HandType PlayHand::getHandType() { return _type; }
Card::CardPoint PlayHand::getCardPoint() { return _point; }
int PlayHand::getExtra() { return _extra; }

bool PlayHand::isSingle() {
    if(_oneCard.size() == 1 && _twoCard.isEmpty() && _threeCard.isEmpty() && _fourCard.isEmpty()) {
        return true;
    }
    return false;
}

bool PlayHand::isPair() {
    if(_oneCard.isEmpty() && _twoCard.size() == 1 && _threeCard.isEmpty() && _fourCard.isEmpty()) {
        return true;
    }
    return false;
}

bool PlayHand::isTriple() {
    if(_oneCard.isEmpty() && _twoCard.isEmpty() && _threeCard.size() == 1 && _fourCard.isEmpty()) {
        return true;
    }
    return false;
}

bool PlayHand::isTripleSingle() {
    if(_oneCard.size() == 1 && _twoCard.isEmpty() && _threeCard.size() == 1 && _fourCard.isEmpty()) {
        return true;
    }
    return false;
}

bool PlayHand::isTriplePair() {
    if(_oneCard.isEmpty() && _twoCard.size() == 1 && _threeCard.size() == 1 && _fourCard.isEmpty()) {
        return true;
    }
    return false;
}

bool PlayHand::isPlane() {
    if(_oneCard.isEmpty() && _twoCard.isEmpty() && _threeCard.size() == 2 && _fourCard.isEmpty()) {
        std::sort(_threeCard.begin(), _threeCard.end()); // 递增序列
        if(_threeCard[1] - _threeCard[0] == 1 && _threeCard[1] < Card::Card_2) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoSingle() {
    if(_oneCard.size() == 2 && _twoCard.isEmpty() && _threeCard.size() == 2 && _fourCard.isEmpty()) {
        std::sort(_threeCard.begin(), _threeCard.end()); // 递增序列
        std::sort(_oneCard.begin(), _oneCard.end());
        if(_threeCard[1] - _threeCard[0] == 1 && _threeCard[1] < Card::Card_2 &&
            _oneCard[0] != Card::Card_SJ && _oneCard[1] != Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoPair() {
    if(_oneCard.isEmpty() && _twoCard.size() == 2 && _threeCard.size() == 2 && _fourCard.isEmpty()) {
        std::sort(_threeCard.begin(), _threeCard.end()); // 递增序列
        if(_threeCard[1] - _threeCard[0] == 1 && _threeCard[1] < Card::Card_2) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqPair() {
    if(_oneCard.isEmpty() && _twoCard.size() >= 3 && _threeCard.isEmpty() && _fourCard.isEmpty()) {
        std::sort(_twoCard.begin(), _twoCard.end());
        if(_twoCard.last() - _twoCard.front() == (_twoCard.size() - 1) &&
            _twoCard.front() >= Card::Card_3 && _twoCard.last() < Card::Card_2) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqSingle() {
    if(_oneCard.size() >= 5 && _twoCard.isEmpty() && _threeCard.isEmpty() && _fourCard.isEmpty()) {
        std::sort(_oneCard.begin(), _oneCard.end());
        if(_oneCard.last() - _oneCard.front() == (_oneCard.size() - 1) &&
            _oneCard.front() >= Card::Card_3 && _oneCard.last() < Card::Card_2) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBomb() {
    if(_oneCard.isEmpty() && _twoCard.isEmpty() && _threeCard.size() == 2 && _fourCard.isEmpty()) {
        std::sort(_threeCard.begin(), _threeCard.end()); // 递增序列
        if(_threeCard[1] - _threeCard[0] == 1 && _threeCard[1] < Card::Card_2) {
            return true;
        }
    }
    return false;
}


