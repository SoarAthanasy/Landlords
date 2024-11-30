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
    _type = Hand_Unknow;
    _point = Card::Card_Begin;
    _extra = 0;
    if(isPass()) {
        _type = Hand_Pass;
    }
    else if(isSingle()) {
        _type = Hand_Single;
        _point = _oneCard.first();
    }
    else if(isPair()) {
        _type = Hand_Pair;
        _point = _twoCard.first();
    }
    else if(isTriple()) {
        _type = Hand_Triple;
        _point = _threeCard.first();
    }
    else if(isTripleSingle()) {
        _type = Hand_Triple_Single;
        _point = _threeCard.first();
    }
    else if(isTriplePair()) {
        _type = Hand_Triple_Pair;
        _point = _threeCard.first();
    }
    else if(isPlane()) {
        _type = Hand_Plane;
        _point = _threeCard.first(); // 记录点数小的那张牌
    }
    else if(isPlaneTwoSingle()) {
        _type = Hand_Plane_Two_Single;
        _point = _threeCard.first(); // 记录点数小的那张牌
    }
    else if(isPlaneTwoPair()) {
        _type = Hand_Plane_Two_Pair;
        _point = _threeCard.first(); // 记录点数小的那张牌
    }
    else if(isSeqPair()) {
        _type = Hand_Seq_Pair;
        _point = _twoCard.first(); // 记录点数小的那张牌
        _extra = _twoCard.size();
    }
    else if(isSeqSingle()) {
        _type = Hand_Seq_Single;
        _point = _oneCard.first(); // 记录点数小的那张牌
        _extra = _oneCard.size();
    }
    else if(isBomb()) {
        _type = Hand_Bomb;
        _point = _fourCard.first(); // 记录点数小的那张牌
    }
    else if(isBombSingle()) {
        _type = Hand_Bomb_Single;
        _point = _fourCard.first(); // 记录点数小的那张牌
    }
    else if(isBombPair()) {
        _type = Hand_Bomb_Pair;
        _point = _fourCard.first(); // 记录点数小的那张牌
    }
    else if(isBombTwoSingle()) {
        _type = Hand_Bomb_Two_Single;
        _point = _fourCard.first(); // 记录点数小的那张牌
    }
    else if(isBombJokers()) {
        _type = Hand_Bomb_Jokers; // 王炸无需记录点数
    }
    else if(isBombJokersSingle()) {
        _type = Hand_Bomb_Jokers_Single;
    }
    else if(isBombJokersPair()) {
        _type = Hand_Bomb_Jokers_Pair;
    }
    else if(isBombJokersTwoSingle()) {
        _type = Hand_Bomb_Jokers_Two_Single;
    }
}

PlayHand::HandType PlayHand::getHandType() { return _type; }
Card::CardPoint PlayHand::getCardPoint() { return _point; }
int PlayHand::getExtra() { return _extra; }

bool PlayHand::canBeat(const PlayHand& other) {
    if(_type == Hand_Unknow) { return false; }     // 当前组合牌的牌型未知
    if(other._type == Hand_Pass) { return true; }  // 对方放弃出牌
    if(_type == Hand_Bomb_Jokers) { return true; } // 王炸 > 其余任何牌型
    if(_type == Hand_Bomb && other._type >= Hand_Single && other._type <= Hand_Seq_Single) {
        // 炸弹 > (单/对子/三/三带一/飞机类/顺子/连对)
        return true;
    }
    if(_type == other._type) { // 双方牌型一致
        if(_type == Hand_Seq_Pair || _type == Hand_Seq_Single) {
            return _point > other._point && _extra == other._extra;
        }
        else {
            return _point > other._point;
        }
    }
    return false;
}

bool PlayHand::isPass() {
    if(_oneCard.isEmpty() && _twoCard.isEmpty() && _threeCard.isEmpty() && _fourCard.isEmpty()) {
        return true;
    }
    return false;
}


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
    if(_oneCard.isEmpty() && _twoCard.isEmpty() && _threeCard.isEmpty() && _fourCard.size() == 1) {
        return true;
    }
    return false;
}

bool PlayHand::isBombSingle() {
    if(_oneCard.size() == 1 && _twoCard.isEmpty() && _threeCard.isEmpty() && _fourCard.size() == 1) {
        return true;
    }
    return false;
}

bool PlayHand::isBombPair() {
    if(_oneCard.isEmpty() && _twoCard.size() == 1 && _threeCard.isEmpty() && _fourCard.size() == 1) {
        return true;
    }
    return false;
}

bool PlayHand::isBombTwoSingle() {
    if(_oneCard.size() == 2 && _twoCard.isEmpty() && _threeCard.isEmpty() && _fourCard.size() == 1) {
        std::sort(_oneCard.begin(), _oneCard.end());
        if(_oneCard.first() != Card::Card_SJ && _oneCard.last() != Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokers() {
    if(_oneCard.size() == 2 && _twoCard.isEmpty() && _threeCard.isEmpty() && _fourCard.isEmpty()) {
        std::sort(_oneCard.begin(), _oneCard.end());
        if(_oneCard.first() == Card::Card_SJ && _oneCard.last() == Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersSingle() {
    if(_oneCard.size() == 3 && _twoCard.isEmpty() && _threeCard.isEmpty() && _fourCard.isEmpty()) {
        std::sort(_oneCard.begin(), _oneCard.end());
        if(_oneCard[1] == Card::Card_SJ && _oneCard[2] == Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersPair() {
    if(_oneCard.size() == 2 && _twoCard.size() == 1 && _threeCard.isEmpty() && _fourCard.isEmpty()) {
        std::sort(_oneCard.begin(), _oneCard.end());
        if(_oneCard.first() == Card::Card_SJ && _oneCard.last() == Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersTwoSingle() {
    if(_oneCard.size() == 4 && _twoCard.isEmpty() && _threeCard.isEmpty() && _fourCard.isEmpty()) {
        std::sort(_oneCard.begin(), _oneCard.end());
        if(_oneCard[2] == Card::Card_SJ && _oneCard[3] == Card::Card_BJ) {
            return true;
        }
    }
    return false;
}
