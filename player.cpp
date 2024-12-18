#include "player.h"

Player::Player(QObject* parent): QObject{parent} {}
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

void Player::setWin(bool flag) { _isWin = flag; }
bool Player::isWin() const { return _isWin; }

void Player::setPrevPlayer(Player* player) { _prev = player; }
void Player::setNextPlayer(Player* player) { _next = player; }
Player *Player::getPrevPlayer() const { return _prev; }
Player *Player::getNextPlayer() const { return _next; }

void Player::grabLordBet(int bet) {
    emit notifyGrabLordBet(this, bet);
}

void Player::storeDispatchCard(Card& card) {
    _cards.add(card);
    Cards cards;
    cards.add(card);
    emit notifyPickCards(this, cards); // 通知GamePanel: 玩家player已经得到了分发的牌card
}

void Player::storeDispatchCards(Cards& cards) {
    _cards.add(cards);
    emit notifyPickCards(this, cards); // 通知GamePanel: 地主player已经得到了3张底牌
}

Cards Player::getCards() { return _cards; }
void Player::clearCards() { _cards.clear(); }

void Player::playHand(Cards& cards) {
    _cards.remove(cards);
    emit notifyPlayHand(this, cards);
}

void Player::setPendingInfo(Player *player, const Cards &cards) {
    _pendPlayer = player;
    _pendCards = cards;
}
Player *Player::getPendPlayer() { return _pendPlayer; }
Cards Player::getPendCards() { return _pendCards; }

void Player::prepareCallLord() { }
void Player::preparePlayHand() { }

void Player::thinkCallLord()
{

}

void Player::thinkPlayHand()
{

}
