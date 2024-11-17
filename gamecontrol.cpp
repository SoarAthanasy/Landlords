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
}

void GameControl::becomeLord(Player* player) {
    // 设置各玩家的角色
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);
    _currPlayer = player;                  // 指定地主为当前玩家
    player->storeDispatchCards(_allCards); // 地主获得剩余的3张底牌
    _currPlayer->preparePlayHand();        // 地主准备出牌
}

void GameControl::clearPlayerScore() {
    _robotLeft->setScore(0);
    _robotRight->setScore(0);
    _user->setScore(0);
}



