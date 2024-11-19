#include "player.h"

Player::Player(QObject* parent): QObject{parent} {
    _score = 0;   _isWin = false;
}
Player::Player(QString name, QObject* parent) : Player(parent) { // Player(parent)是使用了委托构造函数
    _name = name;
}

void Player::setName(QString name) { _name = name; }
QString Player::getName() const { return _name; }

void Player::setRole(Role role) { _role = role; }
Player::Role Player::getRole() const { return _role; }

void Player::setSex(Sex sex) { _sex = sex; }
Player::Sex Player::getSex() const { return _sex; }

void Player::setDirection(Direction direction) { _direction = direction; }
Player::Direction Player::getDirection() const { return _direction; }

void Player::setType(Type type) { _type = type; }
Player::Type Player::getType() const { return _type; }

void Player::setScore(int score) { _score = score; }
int Player::getScore() const { return _score; }

void Player::setWin(bool flag) { _isWin = true; }
bool Player::isWin() const { return _isWin; }

void Player::setPrevPlayer(Player* player) { _prev = player; }
void Player::setNextPlayer(Player* player) { _next = player; }
Player *Player::getPrevPlayer() const { return _prev; }
Player *Player::getNextPlayer() const { return _next; }

void Player::grabLordBet(int point) { }

void Player::storeDispatchCard(Card& card) { _cards.add(card); }
void Player::storeDispatchCards(Cards& cards) { _cards.add(cards); }

Cards Player::getCards() { return _cards; }
void Player::clearCards() { _cards.clear(); }

void Player::playHand(Cards& cards) { _cards.remove(cards); }

void Player::setPendingInfo(Player *player, Cards &cards) {
    _pendPlayer = player;
    _pendCards = cards;
}
Player *Player::getPendPlayer() { return _pendPlayer; }
Cards Player::getPendCards() { return _pendCards; }

void Player::prepareCallLord() { }
void Player::preparePlayHand() { }
