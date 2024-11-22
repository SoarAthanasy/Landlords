#include "gamecontrol.h"

GameControl::GameControl(QObject *parent): QObject{parent} { }

void GameControl::playerInit() {
    // 对象实例化
    _robotLeft = new Robot("机器人A", this);
    _robotRight = new Robot("机器人B", this);
    _user = new User("用户玩家", this);
    // 头像的显示方位
    _robotLeft->setDirection(Player::Right);
    _robotRight->setDirection(Player::Left);
    _user->setDirection(Player::Right);
    // 性别
    Player::Sex sex;
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    _robotLeft->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    _robotRight->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    _user->setSex(sex); // 用户的性别应自己设置, 这里先这样处理

    // 通过上下家制定出牌顺序
    // user
    _user->setPrevPlayer(_robotLeft);
    _user->setNextPlayer(_robotRight);
    // left robot
    _robotLeft->setPrevPlayer(_robotRight);
    _robotLeft->setNextPlayer(_user);
    // right robot
    _robotRight->setPrevPlayer(_user);
    _robotRight->setNextPlayer(_robotLeft);

    _currPlayer = _user; // 指定用户为当前玩家!!

    // 处理玩家发射的信号
    connect(_user, &User::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(_robotLeft, &User::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(_robotRight, &User::notifyGrabLordBet, this, &GameControl::onGrabBet);
}

Robot* GameControl::getLeftRobot() { return _robotLeft; }
Robot* GameControl::getRightRobot() { return _robotRight; }
User* GameControl::getUser() { return _user; }

void GameControl::setCurrPlayer(Player *player) { _currPlayer = player; }
Player* GameControl::getCurrPlayer() { return _currPlayer; }

Player* GameControl::getPendPlayer() { return _pendPlayer; }
Cards GameControl::getPendCards() { return _pendCards; }

void GameControl::initAllCards() {
    _allCards.clear(); // 清空牌
    // 重新加载牌
    // 注意: 枚举类型不能进行算数运算, ∴用int定义p和s, 以支持算术运算, 使用p和s时, 只需再将其强转为枚举类型即可
    for(int p = Card::Card_Begin + 1; p < Card::Card_SJ; ++p) {
        for(int s = Card::Suit_Begin + 1; s < Card::Suit_End; ++s) {
            Card c((Card::CardPoint)p, (Card::CardSuit)s);
            _allCards.add(c);
        }
    }
    _allCards.add(Card(Card::Card_SJ, Card::Suit_Begin));
    _allCards.add(Card(Card::Card_BJ, Card::Suit_Begin));
}

Card GameControl::takeOneCard() { return _allCards.takeRandomCard(); }
Cards GameControl::getSurplusCards() { return _allCards; }

void GameControl::resetCardData() {
    initAllCards(); // 洗牌
    // 清空玩家手里的牌
    _robotLeft->clearCards();
    _robotRight->clearCards();
    _user->clearCards();
    // 清空出牌玩家和打出的牌
    _pendPlayer = nullptr;
    _pendCards.clear();
}

void GameControl::startLordCard() {
    _currPlayer->prepareCallLord(); // 当前玩家准备叫地主: 这里默认为用户玩家准备叫地主
    emit playerStatusChanged(_currPlayer, ThinkingForCallLord);
}

int GameControl::getPlayerMaxBet() {
    return _betRecord._bet;
}

void GameControl::becomeLord(Player* player) {
    // 设置各玩家的角色
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);

    _currPlayer = player;                  // 指定地主为当前玩家
    player->storeDispatchCards(_allCards); // 地主获得剩余的3张底牌
    QTimer::singleShot(1000, this, [=](){
        emit gameStatusChanged(PlayingHand); // 通知主窗口，游戏状态改变为出牌状态
        emit playerStatusChanged(player, ThinkingForPlayHand);
        _currPlayer->preparePlayHand();        // 地主准备出牌
    });
}

void GameControl::clearPlayerScore() {
    _robotLeft->setScore(0);
    _robotRight->setScore(0);
    _user->setScore(0);
}

void GameControl::onGrabBet(Player *player, int bet) {
    // 1. 通知主界面，玩家叫地主了，要更新提示信息了
    if(bet == 0 || _betRecord._bet >= bet) {
        emit notifyGrabLordBet(player, 0, false);
    }
    else if(bet > 0 && _betRecord._bet == 0) { // 当前玩家是第一个抢地主的玩家
        emit notifyGrabLordBet(player, bet, true);
    }
    else { // 第2, 3个抢地主的玩家
        emit notifyGrabLordBet(player, bet, false);
    }
    // 2. 判断玩家下注是不是3分, 若是则该玩家成为地主, 抢地主结束
    if(bet == 3) {
        becomeLord(player); // 玩家成为地主
        _betRecord.reset(); // 清空数据
        return;
    }
    // 3. 所有玩家的下注都不到3分, ∴比较玩家的下注分数, 分数高的是地主
    if(_betRecord._bet < bet) {
        _betRecord._bet = bet;
        _betRecord._player = player;
    }
    _betRecord._times++;
    // 若每个玩家都抢过1次地主，则抢地主结束
    if(_betRecord._times == 3) {
        if(_betRecord._bet == 0) { // 3位玩家都放弃下注, 游戏状态应退回为发牌状态
            emit gameStatusChanged(Dispatch);
        }
        else { // 若有玩家下注, 则让下注分数最高的玩家成为地主
            becomeLord(_betRecord._player);
        }
        _betRecord.reset(); // 不影响下局游戏的下注信息记录
        return;
    }
    // 4. 切换玩家, 通知下一个玩家抢地主
    _currPlayer = player->getNextPlayer();
    // 发送信号给主窗口, 告知当前状态仍为抢地主状态，但当前玩家改变了
    emit playerStatusChanged(_currPlayer, ThinkingForCallLord);
    _currPlayer->prepareCallLord();
}



