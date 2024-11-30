#include "gamepanel.h"
#include "ui_gamepanel.h"

GamePanel::GamePanel(QWidget *parent): QMainWindow(parent), ui(new Ui::GamePanel) {
    ui->setupUi(this);
    // 1. 画背景图
    int index = QRandomGenerator::global()->bounded(1, 11);          // 获取图片索引[1, 11)
    QString path = QString(":/images/background-%1.png").arg(index); // 生成图片路径
    _bkImage.load(path); // 载入图片
    // 2. 窗口标题的大小
    this->setWindowTitle("欢乐斗地主");
    this->setFixedSize(1000, 650);
    // 3. 实例化游戏控制类对象
    initGameControl();
    // 4. (更新)玩家得分
    updatePlayerScore();
    // 5. 切割并保存卡牌图片
    initCardMap();
    // 6. 初始化游戏中的按钮组
    initButtonGroup();
    // 7. 初始化玩家在窗口中的上下文环境
    initPlayerContext();
    // 8. 扑克牌场景初始化
    initGameScene();

    _timer = new QTimer(this); // 实例化定时器
    connect(_timer, &QTimer::timeout, this, &GamePanel::onDispatchCard);

    _animation = new AnimationWindow(this);
}

GamePanel::~GamePanel() { delete ui; }

void GamePanel::initGameControl() {
    _gameControl = new GameControl(this); // 实例化游戏控制类对象
    _gameControl->playerInit(); // 实例化并且初始化玩家
    // 得到三个玩家的实例化对象
    Robot* leftRobot = _gameControl->getLeftRobot();
    Robot* rightRobot = _gameControl->getRightRobot();
    User* user = _gameControl->getUser();
    // 存储的顺序: 左侧机器人, 右侧机器人, 用户玩家
    _playerList << leftRobot << rightRobot << user;

    connect(_gameControl, &GameControl::playerStatusChanged, this, &GamePanel::onPlayerStatucChanged);
    connect(_gameControl, &GameControl::notifyGrabLordBet, this, &GamePanel::onGrabLordBet);
    connect(_gameControl, &GameControl::gameStatusChanged, this, &GamePanel::gameStatusPrecess);
}

void GamePanel::updatePlayerScore() {
    ui->scorePanel->setScores(_playerList[0]->getScore(),
                              _playerList[1]->getScore(),
                              _playerList[2]->getScore());
}

void GamePanel::initCardMap() {
    // 1. 加载大图
    QPixmap pixmap(":/images/card.png");
    // 2. 计算每张图片的大小
    _cardSize.setWidth(pixmap.width() / 13);  // 单张卡牌图片的宽度
    _cardSize.setHeight(pixmap.height() / 5); // 单张卡牌图片的高度
    // 3. 取出单张卡牌的图片
    // 背面图
    _cardBackImage = pixmap.copy(2 * _cardSize.width(), 4 * _cardSize.height(), // 复制区域的起始坐标
                                 _cardSize.width(), _cardSize.height());
    // 正常花色
    for(int i = 0, suit = Card::Suit_Begin + 1; suit < Card::Suit_End; ++i, ++suit) {
        for(int j = 0, point = Card::Card_Begin + 1; point < Card::Card_SJ; ++j, ++point) {
            Card card((Card::CardPoint)point, (Card::CardSuit)suit); // 获取每张正常花色的扑克牌的数据
            // 裁剪图片
            cropImage(pixmap, j * _cardSize.width(), i * _cardSize.height(), card);
        }
    }
    // 大小王
    Card card;
    card.setSuit(Card::Suit_Begin); // 大小王卡牌的花色
    card.setPoint(Card::Card_SJ);   // 小王卡牌的点数
    cropImage(pixmap, 0, 4 * _cardSize.height(), card); // 剪裁小王图片
    card.setPoint(Card::Card_BJ);   // 大王卡牌的点数
    cropImage(pixmap, _cardSize.width(), 4 * _cardSize.height(), card); // 剪裁大王图片
}

void GamePanel::cropImage(QPixmap &pix, int x, int y, Card& card) {
    QPixmap sub = pix.copy(x, y, _cardSize.width(), _cardSize.height()); // 得到剪裁区域的图片
    CardPanel* panel = new CardPanel(this); // 实例化卡牌窗口对象
    panel->setImage(sub, _cardBackImage);   // 给卡牌窗口对象设置正面和反面图片
    panel->setCard(card);                   // 给卡牌窗口对象设置对应卡牌的信息
    panel->hide();                          // 隐藏卡牌窗口, ∵现在还不需要让其显示
    _cardMap.insert(card, panel);           // 将<卡牌, 对应的卡牌窗口>存入_cardMap中
}

void GamePanel::initButtonGroup() {
    ui->btnGroup->initButtons(); // 初始化按钮组窗口类
    ui->btnGroup->selectPage(ButtonGroup::Start); // 显示开始界面
    connect(ui->btnGroup, &ButtonGroup::startGame, this, [=](){
        // 界面的初始化
        ui->btnGroup->selectPage(ButtonGroup::Empty);
        _gameControl->clearPlayerScore();
        updatePlayerScore();
        // 修改游戏状态 → 发牌
        gameStatusPrecess(GameControl::Dispatch);
    });
    connect(ui->btnGroup, &ButtonGroup::playHand, this, [=](){ });
    connect(ui->btnGroup, &ButtonGroup::pass, this, [=](){ });
    connect(ui->btnGroup, &ButtonGroup::betPoint, this, [=](int bet){
        _gameControl->getUser()->grabLordBet(bet); // 让用户下注bet分来抢地主
    });
}

void GamePanel::initPlayerContext() {
    // 1. 放置玩家扑克牌的区域
    QRect cardsRect[] = {
        // x, y, width, height
        QRect(90, 130, 100, height() - 200),                   // 左侧机器人
        QRect(rect().right() - 190, 130, 100, height() - 200), // 右侧机器人
        QRect(250, rect().bottom() - 120, width() - 500, 100)  // 当前玩家
    };
    // 2. 玩家出牌的区域
    QRect playHandRect[] = {
        QRect(260, 150, 100, 100),                             // 左侧机器人
        QRect(rect().right() - 360, 150, 100, 100),            // 右侧机器人
        QRect(150, rect().bottom() - 290, width() - 300, 105)  // 当前玩家
    };
    // 3. 玩家头像显示的位置
    QPoint roleImgPos[] = {
        QPoint(cardsRect[0].left() - 80, cardsRect[0].height() / 2 + 20),  // 左侧机器人
        QPoint(cardsRect[1].right() + 10, cardsRect[1].height() / 2 + 20), // 右侧机器人
        QPoint(cardsRect[2].right() - 10, cardsRect[2].top() - 10)         // 当前玩家
    };
    // 循环遍历
    int index = _playerList.indexOf(_gameControl->getUser());
    for(int i = 0; i < _playerList.size(); ++i) {
        PlayerContext context;
        context.align = (i == index ? Horizontal : Vertical); // 卡牌的对齐方式
        context.isFrontSide = (i == index ? true : false);    // 卡牌显示正面还是反面
        context.cardRect = cardsRect[i];
        context.playCardRect = playHandRect[i];
        // 提示信息
        context.info = new QLabel(this);
        context.info->resize(160, 98);
        context.info->hide(); // 提示信息先隐藏
        // 提示信息显示到出牌区域的中心位置
        QRect rect = playHandRect[i];
        QPoint center(rect.left() + (rect.width() - context.info->width()) / 2,
                      rect.top() + (rect.height() - context.info->height()) / 2);
        context.info->move(center);
        // 玩家头像的显示
        context.roleImg = new QLabel(this);
        context.roleImg->resize(84, 120);
        context.roleImg->hide();              // 玩家头像先隐藏
        context.roleImg->move(roleImgPos[i]); // 移动到计划位置
        _contextMap.insert(_playerList.at(i), context);
    }
}

void GamePanel::initGameScene() {
    // 1. 发牌区的扑克牌(只显示背面图片)
    _baseCard = new CardPanel(this);
    _baseCard->setImage(_cardBackImage, _cardBackImage);
    // 2. 发牌过程中移动的扑克牌
    _moveCard = new CardPanel(this);
    _moveCard->setImage(_cardBackImage, _cardBackImage);
    // 3. 最后的3张底牌(用于窗口的显示)
    for(int i = 0; i < 3; ++i) {
        CardPanel* panel = new CardPanel(this);
        panel->setImage(_cardBackImage, _cardBackImage);
        _last3Card.push_back(panel);
        panel->hide();
    }
    // 扑克牌的位置
    _baseCardPos = QPoint((width() - _cardSize.width()) / 2,
                          (height() - _cardSize.height()) / 2 - 100);
    _baseCard->move(_baseCardPos); // 发牌区扑克牌的位置
    _moveCard->move(_baseCardPos); // 发牌过程中移动扑克牌的起始位置
    int base = (width() - 3 * _cardSize.width() - 2 * 10) / 2;
    for(int i = 0; i < 3; ++i) {
        _last3Card[i]->move(base + (_cardSize.width() + 10) * i, 20);
    }
}

void GamePanel::gameStatusPrecess(GameControl::GameStatus status) { // 处理游戏的状态
    _gameStatus = status; // 记录游戏状态
    // 处理游戏状态
    switch (_gameStatus) {
    case GameControl::Dispatch:
        startDispatchCard();
        break;
    case GameControl::CallingLord:
    {
        // 取出底牌数据(以有序序列的形式)
        CardList last3Card = _gameControl->getSurplusCards().toCardList();
        // 给底牌窗口设置图片
        for(int i = 0; i < last3Card.size(); ++i) {
            QPixmap front = _cardMap[last3Card.at(i)]->getImage();
            _last3Card[i]->setImage(front, _bkImage);
            _last3Card[i]->hide();
        }
        // 开始叫地主
        _gameControl->startLordCard();
        break;
    }
    case GameControl::PlayingHand:
        break;
    default:
        break;
    }
}

void GamePanel::onPlayerStatucChanged(Player *player, GameControl::PlayerStatus status) {
    switch (status) {
    case GameControl::ThinkingForCallLord:
    {
        if(player == _gameControl->getUser()) {
            ui->btnGroup->selectPage(ButtonGroup::CallLord, _gameControl->getPlayerMaxBet());
        }
        else {
            ui->btnGroup->selectPage(ButtonGroup::Empty);
        }
        break;
    }
    case GameControl::ThinkingForPlayHand:
        break;
    case GameControl::Winning:
        break;
    default:
        break;
    }
}

void GamePanel::startDispatchCard() { // 开始发牌
    // 重置每张卡牌的窗口的属性
    for(auto it = _cardMap.begin(); it != _cardMap.end(); ++it) {
        it.value()->setSelected(false); // 每张卡牌的窗口被取消选中状态
        it.value()->setFrontSide(true); // 每张卡牌的窗口显示正面
        it.value()->hide();             // 隐藏每张卡牌的窗口
    }
    // 隐藏3张底牌
    for(int i = 0; i < _last3Card.size(); ++i) {
        _last3Card.at(i)->hide();
    }
    // 重置玩家的窗口上下文信息
    int index = _playerList.indexOf(_gameControl->getUser()); // 获取用户玩家在_playerList中的索引
    for(int i = 0; i < _playerList.size(); ++i) {
        _contextMap[_playerList.at(i)].lastCards.clear();
        _contextMap[_playerList.at(i)].info->hide();
        _contextMap[_playerList.at(i)].roleImg->hide();
        _contextMap[_playerList.at(i)].isFrontSide = (i == index ? true : false);
    }
    // 重置所有玩家的卡牌数据
    _gameControl->resetCardData();
    // 显示底牌
    _baseCard->show();
    // 隐藏按钮面板
    ui->btnGroup->selectPage(ButtonGroup::Empty);
    // 启动定时器 → 开始发牌
    _timer->start(10); // 每隔10ms，定时器就会触发1次
    // 播放背景音乐
}

void GamePanel::onDispatchCard() { // 发牌启动的定时器的处理动作
    // 记录扑克牌的位置
    static int curMovePos = 0;
    // 当前玩家
    Player* curPlayer = _gameControl->getCurrPlayer();
    if(curMovePos >= 100) { // 完成对某玩家的1次发牌动画，现在需要给玩家实际发一张牌，并切换发牌动画的对象
        // 给玩家发一张牌
        Card card = _gameControl->takeOneCard();
        curPlayer->storeDispatchCard(card);
        Cards cards(card);
        disposCard(curPlayer, cards);
        // 切换玩家
        _gameControl->setCurrPlayer(curPlayer->getNextPlayer());
        curMovePos = 0;
        cardMoveStep(curPlayer, curMovePos); // 发牌动画
        // 判断牌是否发完了
        if(_gameControl->getSurplusCards().cardCount() == 3) {
            _timer->stop(); // 终止定时器
            gameStatusPrecess(GameControl::CallingLord); // 切换游戏状态
            return; // 发牌结束
        }
    }
    // 移动扑克牌
    cardMoveStep(curPlayer, curMovePos); // 发牌动画
    curMovePos += 15;
}

void GamePanel::cardMoveStep(Player* player, int curPos) {
    // 得到每个玩家扑克牌的展示区域
    QRect cardRect = _contextMap[player].cardRect;
    // 每个玩家的单元步长
    int unit[] = {
        (_baseCardPos.x() - cardRect.right()) / 100,
        (cardRect.left() - _baseCardPos.x()) / 100,
        (cardRect.top() - _baseCardPos.y()) / 100
    };
    // 每次卡牌窗口移动时, 每个玩家对应的牌的实时坐标
    QPoint pos[] = {
        QPoint(_baseCardPos.x() - curPos * unit[0], _baseCardPos.y()), // 左侧机器人
        QPoint(_baseCardPos.x() + curPos * unit[1], _baseCardPos.y()), // 右侧机器人
        QPoint(_baseCard->x(), _baseCardPos.y() + curPos * unit[2])    // 当前玩家
    };
    // 移动扑克牌窗口
    int index = _playerList.indexOf(player);
    _moveCard->move(pos[index]);
    // 临界状态处理
    if(curPos == 0) { // 卡牌窗口开始移动 → ∴要显示
        _moveCard->show();
    }
    if(curPos >= 100) { // 卡牌窗口移动到终点 → 要隐藏
        _moveCard->hide();
    }
}

void GamePanel::disposCard(Player *player, Cards &cards) {
    Cards& myCard = const_cast<Cards&>(cards);
    CardList list = myCard.toCardList();
    for(int i = 0; i < list.size(); ++i) {
        CardPanel* panel = _cardMap[list.at(i)];
        panel->setOwner(player);
    }
    // 更新扑克牌在窗口中的显示
    updatePlayerCards(player);
}

void GamePanel::updatePlayerCards(Player* player) { // 更新扑克牌在窗口中的显示
    Cards cards = player->getCards();
    CardList list = cards.toCardList();
    // 取出展示扑克牌的区域
    int cardSpace = 20; // 显示的卡牌之间的间隙
    QRect cardsRect = _contextMap[player].cardRect;
    for(int i = 0; i < list.size(); ++i) {
        CardPanel* panel = _cardMap[list.at(i)];
        panel->show();
        panel->raise(); // 让panel窗口处于父窗口的子窗口中最上层的哪一个
        panel->setFrontSide(_contextMap[player].isFrontSide); // 获取窗口显示正面还是背面(玩家是正面，机器人是背面)
        // (水平 or 垂直)显示
        if(_contextMap[player].align == Horizontal) { // 玩家的手牌的显示
            // 玩家显示手牌区域的左上角坐标点
            int leftX = cardsRect.left() + (cardsRect.width() - (list.size() - 1) * cardSpace - panel->width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - _cardSize.height()) / 2;
            if(panel->isSelected()) { // 若卡牌被选中了，让其上移一段距离
                topY -= 10;
            }
            panel->move(leftX + cardSpace * i, topY);
        }
        else {
            int leftX = cardsRect.left() + (cardsRect.width() - _cardSize.width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
            panel->move(leftX, topY + cardSpace * i);
        }
    }
}

void GamePanel::onGrabLordBet(Player *player, int bet, bool flag) {
    // 显示抢地主的信息提示
    PlayerContext context = _contextMap[player];
    if(bet == 0) {
        context.info->setPixmap(QPixmap(":/images/buqinag.png"));
    }
    else {
        if(flag) { // 第一次抢地主
            context.info->setPixmap(QPixmap(":/images/jiaodizhu.png"));
        }
        else { // 第2,3次抢地主
            context.info->setPixmap(QPixmap(":/images/qiangdizhu.png"));
        }
    }
    context.info->show();
    // 显示抢地主的分数
    // 播放分数的背景音乐
}

void GamePanel::paintEvent(QPaintEvent *pe) {
    Q_UNUSED(pe);
    QPainter p(this);
    p.drawPixmap(rect(), _bkImage);
}
