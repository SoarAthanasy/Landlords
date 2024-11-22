#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include <QPainter>
#include <QRandomGenerator>
#include "gamecontrol.h"
#include "cardpanel.h"
#include <QLabel>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class GamePanel; }
QT_END_NAMESPACE

class GamePanel : public QMainWindow {
    Q_OBJECT
public:
    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();

    // 初始化游戏------------------------------------------------------------
    void initGameControl();   // 实例化并初始化游戏控制类对象, 并将三个玩家的实例化对象存入_playerList
    void updatePlayerScore(); // 更新分数面板中各玩家的分数
    void initCardMap();       // 切割并存储单张卡牌的图片
    void cropImage(QPixmap& pix, int x, int y, Card& card); // 剪裁图片
    void initButtonGroup();   // 初始化游戏按钮组
    void initPlayerContext(); // 初始化玩家在窗口中的上下文环境
    void initGameScene();     // 初始化游戏场景

    // 状态变化--------------------------------------------------------------
    void gameStatusPrecess(GameControl::GameStatus status); // 处理游戏的状态
    void onPlayerStatucChanged(Player* player, GameControl::PlayerStatus status); // 处理玩家的状态变化

    // 发牌阶段--------------------------------------------------------------
    void startDispatchCard();                      // 开始发牌
    void onDispatchCard();                         // 发牌启动的定时器的处理动作
    void cardMoveStep(Player* player, int curPos); // 移动扑克牌
    void disposCard(Player* player, Cards& cards); // 处理分发得到的扑克牌
    void updatePlayerCards(Player* player);        // 更新扑克牌在窗口中的显示

    // 抢地主阶段------------------------------------------------------------
    void onGrabLordBet(Player* player, int bet, bool flag);
protected:
    void paintEvent(QPaintEvent* pe);
private:
    enum CardAlign { Horizontal, Vertical };
    struct PlayerContext {  // 玩家在窗口中的上下文环境
        QRect cardRect;     // 1. 玩家扑克牌显示的区域
        QRect playCardRect; // 2. 出牌的区域
        CardAlign align;    // 3. 扑克牌的对齐方式(水平or垂直)
        bool isFrontSide;   // 4. 扑克牌显示正面还是背面
        QLabel* info;       // 5. 游戏过程中的提示信息, 比如: 不出
        QLabel* roleImg;    // 6. 玩家的图像
        Cards lastCards;    // 7. 玩家刚打出的牌
    };

    Ui::GamePanel *ui;
    QPixmap _bkImage;                //  背景图片
    GameControl* _gameControl;       // 游戏控制类的对象
    QVector<Player*> _playerList;    // 三个玩家的实例化对象, 存储顺序: 左侧机器人、右侧机器人、用户玩家
    QMap<Card, CardPanel*> _cardMap; // <单张卡牌, 单张卡牌对应的窗口>
    QSize _cardSize;                 // 单张扑克牌的大小
    QPixmap _cardBackImage;          // 单张卡牌的背面图
    QMap<Player*, PlayerContext> _contextMap; // <玩家, 玩家在窗口中的上下文环境>
    CardPanel* _baseCard; // 发牌区的扑克牌
    CardPanel* _moveCard; // 发牌过程中移动的扑克牌
    QVector<CardPanel*> _last3Card; // 最后三张底牌
    QPoint _baseCardPos;  // 发牌区扑克牌的位置
    GameControl::GameStatus _gameStatus; // 游戏的状态: 发牌、抢地主、出牌
    QTimer* _timer; // 定时器
};
#endif // GAMEPANEL_H
