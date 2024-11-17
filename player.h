#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"

class Player : public QObject {
    Q_OBJECT
public:
    enum Role { Lord, Farmer }; // 角色
    enum Sex { Man, Woman };    // 性别
    enum Direction { Left, Right };    // 头像的显示方向
    enum Type { Robot, User, UnKnow }; // 玩家的类型

    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name, QObject *parent = nullptr);
    // 名字
    void setName(QString name);
    QString getName() const;
    // 角色
    void setRole(Role role);
    Role getRole() const;
    // 性别
    void setSex(Sex sex);
    Sex getSex() const;
    // 头像的显示方向
    void setDirection(Direction direction);
    Direction getDirection() const;
    // 玩家的类型
    void setType(Type type);
    Type getType() const;
    // 分数
    void setScore(int score);
    int getScore() const;
    // 游戏结果
    void setWin(bool flag);
    bool isWin() const;
    // 提供当前玩家的(上家/下家)
    void setPrevPlayer(Player* player);
    void setNextPlayer(Player* player);
    Player* getPrevPlayer() const;
    Player* getNextPlayer() const;
    // 叫地主/抢地主。grab-抓取、bet-下注
    void grabLordBet(int point);
    // 存储发来的扑克牌(发牌的时候得到的)。dispatch-发送
    void storeDispatchCard(Card& card);
    void storeDispatchCards(Cards& cards); // 地主会得到最后的3张牌

    Cards getCards();  // 得到玩家手中所有的牌(用于显示)
    void clearCards(); // 清空玩家手中所有的牌(游戏结束时调用)

    void playHand(Cards& cards); // 出牌

    // 待处理的牌及其所属玩家
    void setPendingInfo(Player* player, Cards& cards);
    Player* getPendPlayer();
    Cards getPendCards();

    virtual void prepareCallLord();
    virtual void preparePlayHand();
signals:
protected:
    int _score;    // 玩家的分数
    QString _name; // 玩家的姓名
    Role _role;    // 玩家的角色
    Sex _sex;      // 玩家的性别
    Direction _direction; // 玩家的头像显示方向
    Type _type;    // 玩家的类型
    bool _isWin;   // 记录玩家是否赢了
    Player* _prev; // 当前玩家的上家
    Player* _next; // 当前玩家的下家
    Cards _cards;  // 玩家手中的牌
    Cards _pendCards;    // 待处理的牌
    Player* _pendPlayer; // 待处理牌的所属玩家
};

#endif // PLAYER_H
