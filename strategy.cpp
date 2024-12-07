#include "strategy.h"

Strategy::Strategy() {}

Strategy::Strategy(Player *player, const Cards &cards) : _player(player), _cards(cards) {}

Cards Strategy::makeStrategy() {
    // 得到上次出牌的玩家以及他打出的牌
    Player* pendPlayer = _player->getPendPlayer();
    Cards pendCards = _player->getPendCards();
    if(pendPlayer == nullptr) {
        qDebug() << _player->getName() << "在makeStrategy()中获得的pendPlayer: NULL";
    }
    else {
        qDebug() << _player->getName() << "在makeStrategy()中获得的pendPlayer:" << pendPlayer->getName();
    }
    // 判断上次出牌的玩家是不是自己
    if(pendPlayer == _player || pendPlayer == nullptr) {
        // 上次出牌的玩家(是我自己/为空): 直接出牌
        qDebug() << _player->getName() << "在makeStrategy()中调用了firstPlay()";
        return firstPlay();
    }
    else { // 上次出牌的玩家不是自己，需要找出比上次出牌玩家点数大的牌
        PlayHand pendType(pendCards);
        qDebug() << _player->getName() << "在makeStrategy()中调用了getGreaterCards()";
        Cards beatCards = getGreaterCards(pendType); // 获取点数大于上次出牌玩家的牌
        // 找到了点数更大的牌, 需要考虑是否出牌
        qDebug() << _player->getName() << "在makeStrategy()中调用了whetherToBeat()";
        bool shouldBeat = whetherToBeat(beatCards);  // 判断是否要打出beatCards
        if(shouldBeat) { return beatCards; }         // 选择打出beatCards
        else { return Cards(); }                     // 选择不打出beatCards, 则返回空对象
    }
    return Cards();
}

Cards Strategy::firstPlay() {
    // 判断自己的手牌是否只剩一个单一的牌型
    PlayHand hand(_cards);
    if(hand.getHandType() != PlayHand::Hand_Unknow) {
        return _cards;
    }

    // 不是单一牌型, 则先判断当前玩家的手牌中是否有顺子
    QVector<Cards> optimalSeq = pickOptimalSeqSingle();
    if( !optimalSeq.isEmpty() ) {
        // 得到_cards打出optimal后的剩余牌的单牌数量
        int baseNum = findCardsByCount(1).size(); // 得到_cards的单牌数量
        Cards save = _cards;
        save.remove(optimalSeq); // 得到 save = _cards - optimal
        int lastNum = Strategy(_player, save).findCardsByCount(1).size(); // 得到save的单牌数量
        if(baseNum > lastNum) { return optimalSeq[0]; }
    }

    bool hasPlane, hasTriple, hasSeqPair, hasBomb;
    hasSeqPair = hasTriple = hasPlane = hasBomb = false;
    Cards backupCards = _cards; // 存储_cards中除取(炸弹/飞机/3张点数相同组合牌/连对)后的剩余牌
    // 有没有炸弹
    QVector<Cards> bombArray = findCardType(PlayHand(PlayHand::Hand_Bomb, Card::Card_Begin, 0), false);
    backupCards.remove(bombArray);
    // 有没有飞机
    QVector<Cards> planeArray = Strategy(_player, backupCards)
                                    .findCardType(PlayHand(PlayHand::Hand_Plane, Card::Card_Begin, 0), false);
    if( !planeArray.isEmpty() ) {
        hasPlane = true;
        backupCards.remove(planeArray);
    }
    // 有没有三张点数相同的牌
    QVector<Cards> tripleArray = Strategy(_player, backupCards).
                                    findCardType(PlayHand(PlayHand::Hand_Triple, Card::Card_Begin, 0), false);
    if( !tripleArray.isEmpty() ) {
        hasTriple = true;
        backupCards.remove(tripleArray);
    }
    // 有没有连对
    QVector<Cards> seqPairArray = Strategy(_player, backupCards).
                                    findCardType(PlayHand(PlayHand::Hand_Seq_Pair, Card::Card_Begin, 0), false);
    if( !seqPairArray.isEmpty() ) {
        hasSeqPair = true;
        backupCards.remove(seqPairArray);
    }
    // 此时backupCards中存在的就是一些散碎的牌，可用于(三带一/飞机带两)

    if(hasSeqPair) { // 优先打出较长的连对
        Cards maxSeqPair;
        for(int i = 0; i < seqPairArray.size(); ++i) {
            if(seqPairArray[i].cardCount() > maxSeqPair.cardCount()) {
                maxSeqPair = seqPairArray[i];
            }
        }
        return maxSeqPair;
    }

    if(hasPlane) { // 飞机优先级第2
        // 1.飞机带两个对子
        bool twoPairFond = false;
        QVector<Cards> pairArray;
        for(Card::CardPoint point = Card::Card_3; point <= Card::Card_Q; point = (Card::CardPoint)(point + 1)) {
            Cards pair = Strategy(_player, backupCards).findSamePointCards(point, 2);
            if( !pair.isEmpty() ) {
                pairArray.push_back(pair);
                if(pairArray.size() == 2) {
                    twoPairFond = true;
                    break;
                }
            }
        }
        if(twoPairFond) {
            Cards tmp = planeArray[0];
            tmp.add(pairArray);
            return tmp;
        }
        else { // 2.飞机带两张单牌
            bool twoSingleFond = false;
            QVector<Cards> SingleArray;
            for(Card::CardPoint point = Card::Card_3; point <= Card::Card_Q; point = (Card::CardPoint)(point + 1)) {
                if(backupCards.pointCount(point) == 1) { // 不破坏非单的牌型
                    Cards single = Strategy(_player, backupCards).findSamePointCards(point, 1);
                    if( !single.isEmpty() ) {
                        pairArray.push_back(single);
                        if(pairArray.size() == 2) {
                            twoSingleFond = true;
                            break;
                        }
                    }
                }
            }
            if(twoSingleFond) {
                Cards tmp = planeArray[0];
                tmp.add(pairArray);
                return tmp;
            }
            else { // 3.飞机
                return planeArray[0];
            }
        }
    }

    if(hasTriple) { // 三带一优先级第三
        if(PlayHand(tripleArray[0]).getCardPoint() < Card::Card_A) { // 若有点数小于A的三带一才会打出
            for(Card::CardPoint point = Card::Card_3; point <= Card::Card_A; point = (Card::CardPoint)(point + 1)) {
                int pointCount = backupCards.pointCount(point); // 获取backupCards中当前点数的牌的数量
                if(pointCount == 1) { // 打出三带一单
                    Cards single = Strategy(_player, backupCards).findSamePointCards(point, 1);
                    Cards tmp = tripleArray[0];
                    tmp.add(single);
                    return tmp;
                }
                else if(pointCount == 2) { // 打出三带一对
                    Cards pair = Strategy(_player, backupCards).findSamePointCards(point, 2);
                    Cards tmp = tripleArray[0];
                    tmp.add(pair);
                    return tmp;
                }
            }
            // 不带副牌
            return tripleArray[0];
        }
    }

    // 单牌或是对牌优先级第4
    Player* nextPlayer = _player->getNextPlayer();
    if(nextPlayer->getCards().cardCount() <= 2 && _player->getRole() != nextPlayer->getRole()) {
        // 此时优先选取较大的(单牌/对子) → 倒序遍历点数
        for(Card::CardPoint point = Card::Card_BJ; point >= Card::Card_3; point = (Card::CardPoint)(point - 1)) {
            int pointCount = backupCards.pointCount(point);
            if(pointCount == 1) { // 该点数存在1张
                Cards single = Strategy(_player, backupCards).findSamePointCards(point, 1);
                return single;
            }
            else if(pointCount == 2) { // 该点数存在2张
                Cards pair = Strategy(_player, backupCards).findSamePointCards(point, 2);
                return pair;
            }
        }
    }
    else { // 此时可出点数较小的(单牌/对子)
        for(Card::CardPoint point = Card::Card_3; point <= Card::Card_BJ; point = (Card::CardPoint)(point + 1)) {
            int pointCount = backupCards.pointCount(point);
            if(pointCount == 1) { // 该点数存在1张
                Cards single = Strategy(_player, backupCards).findSamePointCards(point, 1);
                return single;
            }
            else if(pointCount == 2) { // 该点数存在2张
                Cards pair = Strategy(_player, backupCards).findSamePointCards(point, 2);
                return pair;
            }
        }
    }
    return Cards();
}

Cards Strategy::getGreaterCards(PlayHand type) {
    // 1. 上次出牌玩家和当前玩家是不是一伙的
    Player* pendPlayer = _player->getPendPlayer();
    if(pendPlayer != nullptr && pendPlayer->getRole() != _player->getRole() && pendPlayer->getCards().cardCount() <= 4) {
        // 上次出牌玩家和当前玩家不是一伙的，且上次出牌玩家的手牌数≤4
        QVector<Cards> bombs = findCardsByCount(4); // 获取_cards中的炸弹
        for(int i = 0; i < bombs.size(); ++i) {
            if( PlayHand(bombs[i]).canBeat(type) ) {
                return bombs[i];
            }
        }
        // 搜索当前玩家手中有没有王炸
        Cards sj = findSamePointCards(Card::Card_SJ, 1);
        Cards bj = findSamePointCards(Card::Card_BJ, 1);
        if(!sj.isEmpty() && !bj.isEmpty()) {
            Cards bombJokers;
            bombJokers << sj << bj;
            return bombJokers;
        }
    }
    // 2. 当前玩家和下一个玩家是不是一伙的
    Player* nextPlayer = _player->getNextPlayer(); // 获取当前玩家的下家
    // 将玩家手中的顺子剔除出去, ∵除非情况危急，一般不会拆开手牌中的顺子
    Cards remain = _cards; // 备份_cards, 用于获取剔除顺子后剩下的牌
    remain.remove(Strategy(_player, remain).pickOptimalSeqSingle());

    auto getbeatCards = std::bind([=](const Cards& cards){ // 定义可调用对象: 获取cards中所有大于待处理牌的组合牌
        QVector<Cards> beatCardsArray = Strategy(_player, cards).findCardType(type, true);
        if( !beatCardsArray.isEmpty() ) { // 若remain中有大于待处理牌的同牌型组合牌
            if(_player->getRole() != nextPlayer->getRole() && nextPlayer->getCards().cardCount() <= 2) {
                // 若[当前玩家和下一个玩家不是一伙的 && 下一个玩家的手牌数≤2], 则打出beatCardsArray中最大的组合牌
                return beatCardsArray.last();
            }
            else { // 否则打出beatCardsArray中最小的组合牌
                return beatCardsArray.first();
            }
        }
        return Cards();
    }, std::placeholders::_1);

    Cards beatCards = getbeatCards(remain);          // 获取remain中大于待处理牌的同牌型组合牌
    if( !beatCards.isEmpty() ) { return beatCards; } // 若remain中有大于待处理牌的同牌型组合牌
    else { // 若remain中没有大于待处理牌的同牌型组合牌, 则获取_cards中所有大于待处理牌的组合牌
        beatCards = getbeatCards(_cards);
        if( !beatCards.isEmpty() ) { return beatCards; }
    }
    return Cards();
}

bool Strategy::whetherToBeat(Cards &cards) {
    if(cards.isEmpty()) { return false; }             // _cards中没有能够击败对方的牌
    Player* pendPlayer = _player->getPendPlayer();    // 得到上次出牌玩家
    if(_player->getRole() == pendPlayer->getRole()) { // 上次出牌玩家和自己都是农民
        // 1. 手牌所剩无几，并且是一个完整的牌型
        Cards left = _cards; // 备份_cards，用于计算
        left.remove(cards);
        if(PlayHand(left).getHandType() != PlayHand::Hand_Unknow) { return true; }
        // 2. 若cards中的最小点数牌是(2/大小王) → 不出牌，将牌用于压地主
        Card::CardPoint basePoint = PlayHand(cards).getCardPoint();
        if(basePoint == Card::Card_2 || basePoint == Card::Card_SJ || basePoint == Card::Card_BJ) {
            return false;
        }
    }
    else { // 出牌玩家和当前玩家不是一伙的
        PlayHand myHand(cards);
        // 若是三个2带(一单/一对) → 不出牌，为了保存实力
        if((myHand.getHandType() == PlayHand::Hand_Triple_Pair || myHand.getHandType() == PlayHand::Hand_Triple_Pair)
            && myHand.getCardPoint() == Card::Card_2)
        {
            return false;
        }
        // 若[cards是对2 && 上次出牌玩家的手牌数量 ≥ 10 && 自己的手牌数量 ≥ 5] → 暂时放弃出牌，为了保存实力
        if(myHand.getHandType() == PlayHand::Hand_Pair && myHand.getCardPoint() == Card::Card_2
            && pendPlayer->getCards().cardCount() >= 10 && _player->getCards().cardCount() >= 5)
        {
            return false;
        }
    }
    return true;
}

Cards Strategy::findSamePointCards(Card::CardPoint point, int count) {
    if(count < 1 || count > 4) { return Cards(); } // 数量非法
    if(point == Card::Card_SJ || point == Card::Card_BJ) { // 大小王
        if(count > 1) { return Cards(); } // 数量非法, 返回空对象
        Card card;
        card.setPoint(point);
        card.setSuit(Card::Suit_Begin);
        if( _cards.contains(card) ) { // 找到了大小王，返回大小王
            return Cards(card);
        }
        return Cards(); // 没找到大小王, 返回空对象
    }
    // 不是大小王
    int findCount = 0;
    Cards findCards;
    for(int suit = Card::Suit_Begin + 1; suit < Card::Suit_End; ++suit) {
        // 创建点数与point相同的各花色的牌，判断其是否在_cards中存在
        Card card;
        card.setPoint(point);
        card.setSuit((Card::CardSuit)suit);
        if(_cards.contains(card)) {
            findCount++;
            findCards.add(card);
            if(findCount == count) { return findCards; }
        }
    }
    return Cards();
}

QVector<Cards> Strategy::findCardsByCount(int count) {
    if(count < 1 || count > 4) { return QVector<Cards>(); }
    QVector<Cards> cardsArray;
    for(Card::CardPoint point = Card::Card_3; point < Card::Card_End; point = (Card::CardPoint)(point + 1)) {
        if(_cards.pointCount(point) == count) {
            Cards cards;
            cards << findSamePointCards(point, count);
            cardsArray <<cards;
        }
    }
    return cardsArray;
}

Cards Strategy::getRangeCards(Card::CardPoint begin, Card::CardPoint end) {
    Cards rangeCards;
    for(Card::CardPoint point = begin; point < end; point = (Card::CardPoint)(point + 1)) {
        int count = _cards.pointCount(point);
        Cards cs = findSamePointCards(point, count);
        rangeCards << cs;
    }
    return rangeCards;
}

QVector<Cards> Strategy::findCardType(PlayHand hand, bool beat) {
    PlayHand::HandType type = hand.getHandType();
    Card::CardPoint point = hand.getCardPoint();
    int extra = hand.getExtra();
    // 确定起始点数
    Card::CardPoint beginPoint = (beat ? Card::CardPoint(point + 1) : Card::Card_3);
    switch(type) {
    case PlayHand::Hand_Single:
        return getCards(beginPoint, 1);
    case PlayHand::Hand_Pair:
        return getCards(beginPoint, 2);
    case PlayHand::Hand_Triple:
        return getCards(beginPoint, 3);
    case PlayHand::Hand_Triple_Single:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Triple_Pair:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Plane:
        return getPlane(beginPoint);
    case PlayHand::Hand_Plane_Two_Single:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Plane_Two_Pair:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Seq_Pair:
    {
        CardInfo info;
        info._begin = beginPoint;
        info._end = Card::Card_Q;
        info._number = 2;
        info._base = 3;
        info._extra = extra;
        info._beat = beat;
        info._getSeq = &Strategy::getBaseSeqPair;
        return getSeqPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Seq_Single:
    {
        CardInfo info;
        info._begin = beginPoint;
        info._end = Card::Card_10;
        info._number = 1;
        info._base = 5;
        info._extra = extra;
        info._beat = beat;
        info._getSeq = &Strategy::getBaseSeqSingle;
        return getSeqPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Bomb:
        return getBomb(beginPoint);
    default:
        return QVector<Cards>();
    }
}

void Strategy::pickSeqSingles(QVector<QVector<Cards>>& allSeqRecord, const QVector<Cards>& seqSingle, const Cards &cards) {
    // 1. 得到cards中所有顺子的集合
    QVector<Cards> allSeqSingles = Strategy(_player, cards)
                                       .findCardType(PlayHand(PlayHand::Hand_Seq_Single,Card::Card_Begin, 0), false);
    if( allSeqSingles.isEmpty() ) {
        allSeqRecord << seqSingle; // 结束递归, 将满足条件的顺子传递给调用者
    }
    else { // 2. 对顺子进行筛选
        Cards saveCards = cards;
        for(int i = 0; i < allSeqSingles.size(); ++i) {
            Cards aScheme = allSeqSingles.at(i); // 将顺子取出
            // 将顺子从用户手中删除
            Cards temp = saveCards;
            temp.remove(aScheme);
            // 获取上级递归已得到的顺子, 并将新得到的顺子加入其中
            QVector<Cards> seqArray = seqSingle;
            seqArray << aScheme;
            // 检测temp中还有没有其他的顺子
            pickSeqSingles(allSeqRecord, seqArray, temp);
            // seqArray: 存储一轮for循环中，多轮递归得到的所有的可用的顺子
            // allSeqRecord: 存储多轮for循环中，多轮递归得到的所有的可用的顺子
        }
    }
}

QVector<Cards> Strategy::pickOptimalSeqSingle() {
    QVector<QVector<Cards>> allSeqRecord; // [_cards - 炸弹 - 三张]中所有可能的顺子集合
    QVector<Cards> seqSingles;            // [_cards - 炸弹 - 三张]中某一个[可能的顺子集合]
    Cards save = _cards;              // save = [_cards -炸弹 - 三张]
    save.remove(findCardsByCount(4)); // 剔除炸弹, 防止在_cards中获取的顺子集合会拆开炸弹，导致因小失大
    save.remove(findCardsByCount(3)); // 剔除三带系列, 防止在_cards中获取的顺子集合会拆开三带系列，导致因小失大
    pickSeqSingles(allSeqRecord, seqSingles, save);
    if(allSeqRecord.isEmpty() || (allSeqRecord.size() == 1 && allSeqRecord.first().isEmpty())) { // 玩家的[_cards -炸弹 - 三张]中一个顺子也没有
        return QVector<Cards>();
    }
    QMap<int, int> seqMarks; // <arrSeqRecord[i]的索引i, allSeqRecord[i]的mark值>
    for(int i = 0; i < allSeqRecord.size(); ++i) { // 遍历[_cards -炸弹 - 三张]中所有可能的顺子集合
        Cards backupCards = save; // ∵要想计算得到最优顺子集合, 但在计算过程中需要修改save，∴先备份save
        QVector<Cards> seqArray = allSeqRecord[i]; // 获取[_cards -炸弹 - 三张]中所有可能的顺子集合中的一个
        backupCards.remove(seqArray); // 获取[save - save中某一个可能的顺子集合]
        // 获取backupCards中的单牌数量，单牌数量越少，则当前被移除的顺子集合越优异
        QVector<Cards> singleArray = Strategy(_player, backupCards).findCardsByCount(1);
        // ∵是单牌，∴最好将QVector<Cards>转化为QVector<Card>，这样便于后续的操作
        CardList cardList;
        for(int j = 0; j< singleArray.size(); ++j) {
            cardList << singleArray[j].toCardList();
        }
        // 找点数相对较大一点的顺子(该算法只是适于普遍情况)
        int mark = 0;
        for(int j = 0; j < cardList.size(); ++j) {
            mark += cardList[j].point() + 15; // 每张牌+15，是因为要让(单3+单4>单K)
        }
        seqMarks.insert(i, mark);
    }
    // 遍历seqMarks, 找出最小mark对应的索引值i
    int value = 0;      // 记录最小mark对应的索引
    int comMark = 1000; // 记录最小mark
    auto it = seqMarks.constBegin();
    for(; it != seqMarks.end(); ++it) {
        if(it.value() < comMark) {
            comMark = it.value();
            value = it.key();
        }
    }
    return allSeqRecord[value];
}

QVector<Cards> Strategy::getCards(Card::CardPoint begin, int number) {
    QVector<Cards> findCardsArray;
    for(Card::CardPoint point = begin; point < Card::Card_End; point = (Card::CardPoint)(point + 1)) {
        if(_cards.pointCount(point) == number) { // 目的是尽量不拆分别的牌型
            Cards cards = findSamePointCards(point, number);
            findCardsArray << cards;
        }
    }
    return findCardsArray;
}

QVector<Cards> Strategy::getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type) {
    // 找到点数相同的3张牌
    QVector<Cards> findCardsArray = getCards(begin, 3);
    if( !findCardsArray.isEmpty() ) {
        // 将找到的3张牌从用户手牌中删除
        Cards remainCards = _cards;
        remainCards.remove(findCardsArray);
        // 搜索单牌或者成对的牌
        Strategy strategy(_player, remainCards);
        QVector<Cards> cardsArray = strategy.findCardType(PlayHand(type, Card::Card_Begin, 0), false);
        if( !cardsArray.isEmpty() ) {
            // 将找到的牌和三张点数相同的牌进行组合
            for(int i = 0; i < findCardsArray.size(); ++i) {
                // 将找到的(单牌/对子)中最小的，与findCardsArray中的元素组合即可
                findCardsArray[i].add(cardsArray.first());
            }
        }
        else { findCardsArray.clear(); }
    }
    // 将所有[得到的有效组合]的集合返回即可
    return findCardsArray;
}

QVector<Cards> Strategy::getPlane(Card::CardPoint begin) {
    QVector<Cards> findCardArray;
    for(Card::CardPoint point = begin; point <= Card::Card_K; point = (Card::CardPoint)(point + 1)) {
        // 根据点数和数量进行搜索
        Cards prevCards = findSamePointCards(point, 3);
        Cards nextCards = findSamePointCards((Card::CardPoint)(point + 1), 3);
        if( !prevCards.isEmpty() && !nextCards.isEmpty() ) {
            Cards tmp;
            tmp << prevCards << nextCards;
            findCardArray << tmp;
        }
    }
    return findCardArray;
}

QVector<Cards> Strategy::getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type) {
    QVector<Cards> findCardsArray = getPlane(begin);
    if( !findCardsArray.isEmpty() ) {
        // 将找到的飞机形式组合牌从用户手牌中删除
        Cards remainCards = _cards;
        remainCards.remove(findCardsArray);
        // 搜索单牌或者成对的牌
        Strategy strategy(_player, remainCards);
        QVector<Cards> cardsArray = strategy.findCardType(PlayHand(type, Card::Card_Begin, 0), false);
        if( cardsArray.size() >= 2 ) {
            // 找到了(2个单牌/2个对子)，将其添加到飞机组合中
            for(int i = 0; i < findCardsArray.size(); ++i) {
                // 将找到的(单牌/对子)中最小的，与findCardsArray中的元素组合即可
                Cards tmp;
                tmp << cardsArray[0] << cardsArray[1];
                findCardsArray[i].add(tmp);
            }
        }
        else { findCardsArray.clear(); }
    }
    return findCardsArray;
}

QVector<Cards> Strategy::getSeqPairOrSeqSingle(CardInfo& info) {
    QVector<Cards> findCardsArray;
    if(info._beat) { // 若beat为true，则连对的[长度需要为extra，点数要大于begin]
        // 最少3个，最大的牌为A
        for(Card::CardPoint point = info._begin; point <= info._end; point = (Card::CardPoint)(point + 1)) {
            bool found = true; // 判断_cards中起始点数为point的牌, 能否构成extra长的连对
            Cards seqCards;    // _cards中起始点数为point所能构成的连对形式组合牌
            for(int i = 0; i < info._extra; ++i) {
                // 基于点数和数量进行牌的搜索
                Cards cards = findSamePointCards((Card::CardPoint)(point + i), info._number);
                if( cards.isEmpty() || (point + info._extra >= Card::Card_2) ) {
                    // _cards中不存在[起始点数为point的 + extra长的]连对形式组合牌
                    found = false;
                    seqCards.clear();
                    break;
                }
                else { seqCards << cards; }
            }
            if(found) {
                findCardsArray << seqCards;
                return findCardsArray; // beat为true，则只要找到[点数 ≥ begin]的最小的extra长的连对即可
            }
        }
    }
    else { // beat为false，则对连对的长度和大小都没有要求了
        for(Card::CardPoint point = info._begin; point <= info._end; point = (Card::CardPoint)(point + 1)) {
            // 先存储找到的基础连对
            Cards baseSeq = (this->*info._getSeq)(point);
            if( baseSeq.isEmpty() ) { continue; }
            findCardsArray << baseSeq; // 基础连对存储到结果容器中
            int followed = info._base; // 以[baseSep为基础连对]的连对的长度
            Cards alreadyFollowedCards; // 存储后续找到的满足条件的连对
            while(true) {
                // 新的起始点数
                Card::CardPoint followedPoint = Card::CardPoint(point + followed);
                if(followedPoint >= Card::Card_2) { // 判断是否超出了上限
                    break;
                }
                Cards followedCards = findSamePointCards(followedPoint, info._number);
                if(followedCards.isEmpty()) { break; }
                else {
                    alreadyFollowedCards << followedCards;
                    Cards newSeq = baseSeq;
                    newSeq << alreadyFollowedCards; // newSeq = baseSep + alreadyFollowedCards
                    findCardsArray << newSeq;
                    followed++;
                }
            }
        }
    }
    return findCardsArray;
}

Cards Strategy::getBaseSeqPair(Card::CardPoint point) {
    Cards cards0 = findSamePointCards(point, 2);
    Cards cards1 = findSamePointCards((Card::CardPoint)(point + 1), 2);
    Cards cards2 = findSamePointCards((Card::CardPoint)(point + 2), 2);
    Cards baseSeq;
    if(cards0.isEmpty() || cards1.isEmpty() || cards2.isEmpty()) { return baseSeq; }
    baseSeq << cards0 << cards1 << cards2;
    return baseSeq;
}

Cards Strategy::getBaseSeqSingle(Card::CardPoint point) {
    Cards cards0 = findSamePointCards(point, 1);
    Cards cards1 = findSamePointCards((Card::CardPoint)(point + 1), 1);
    Cards cards2 = findSamePointCards((Card::CardPoint)(point + 2), 1);
    Cards cards3 = findSamePointCards((Card::CardPoint)(point + 3), 1);
    Cards cards4 = findSamePointCards((Card::CardPoint)(point + 4), 1);
    Cards baseSeq;
    if(cards0.isEmpty() || cards1.isEmpty() || cards2.isEmpty() || cards3.isEmpty() || cards4.isEmpty()) {
        return baseSeq;
    }
    baseSeq << cards0 << cards1 << cards2 << cards3 << cards4;
    return baseSeq;
}

QVector<Cards> Strategy::getBomb(Card::CardPoint begin) {
    QVector<Cards> findCardsArray;
    for(Card::CardPoint point = begin; point < Card::Card_End; point = (Card::CardPoint)(point + 1)) {
        Cards cards = findSamePointCards(point, 4);
        if( !cards.isEmpty() ) {
            findCardsArray << cards;
        }
    }
    return findCardsArray;
}










