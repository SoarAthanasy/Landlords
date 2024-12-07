#include "gamepanel.h"
#include "ui_gamepanel.h"
#include "playhand.h"
#include <QRandomGenerator>
#include <QMouseEvent>
#include "endingpanel.h"
#include <QPropertyAnimation>

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
    // 9. 初始化出牌倒计时窗口
    initCountDown();

    _timer = new QTimer(this); // 实例化定时器
    connect(_timer, &QTimer::timeout, this, &GamePanel::onDispatchCard);

    _animation = new AnimationWindow(this);
    _bgm = new BGMControl(this);
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
    // 将GameControl的信号与GamePanel对应的槽建立连接
    connect(_gameControl, &GameControl::playerStatusChanged, this, &GamePanel::onPlayerStatucChanged);
    connect(_gameControl, &GameControl::notifyGrabLordBet, this, &GamePanel::onGrabLordBet);
    connect(_gameControl, &GameControl::gameStatusChanged, this, &GamePanel::gameStatusPrecess);
    connect(_gameControl, &GameControl::notifyPlayHand, this, &GamePanel::onDisposePlayHand);

    connect(leftRobot, &Player::notifyPickCards, this, &GamePanel::disposCard);
    connect(rightRobot, &Player::notifyPickCards, this, &GamePanel::disposCard);
    connect(user, &Player::notifyPickCards, this, &GamePanel::disposCard);
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
    connect(panel, &CardPanel::cardSelected, this, &GamePanel::onCardSelected);
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
        // 播放背景音乐
        _bgm->startBGM(80);
    });
    connect(ui->btnGroup, &ButtonGroup::playHand, this, &GamePanel::onUserPlayHand);
    connect(ui->btnGroup, &ButtonGroup::pass, this, &GamePanel::onUserPass);
    connect(ui->btnGroup, &ButtonGroup::betPoint, this, [=](int bet){
        _gameControl->getUser()->grabLordBet(bet); // 让用户下注bet分来抢地主
        ui->btnGroup->selectPage(ButtonGroup::Empty);
    });
}

void GamePanel::initPlayerContext() {
    // 1. 放置玩家扑克牌的区域
    const QRect cardsRect[] = {
        // x, y, width, height
        QRect(90, 130, 100, height() - 200),                   // 左侧机器人
        QRect(rect().right() - 190, 130, 100, height() - 200), // 右侧机器人
        QRect(250, rect().bottom() - 120, width() - 500, 100)  // 当前玩家
    };
    // 2. 玩家出牌的区域
    const QRect playHandRect[] = {
        QRect(260, 150, 100, 100),                             // 左侧机器人
        QRect(rect().right() - 360, 150, 100, 100),            // 右侧机器人
        QRect(150, rect().bottom() - 290, width() - 300, 105)  // 当前玩家
    };
    // 3. 玩家头像显示的位置
    const QPoint roleImgPos[] = {
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
                          height() / 2 - 100);
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
    {
        // 1. 隐藏发牌区的底牌和移动的牌
        _baseCard->hide();
        _moveCard->hide();
        // 2. 显示留给地主的三张牌
        for(int i = 0; i < _last3Card.size(); ++i) {
            _last3Card[i]->show();
        }
        // 3. 隐藏各个玩家抢地主过程中的提示信息
        for(int i = 0; i < _playerList.size(); ++i) {
            PlayerContext &context = _contextMap[_playerList[i]];
            context.info->hide();
            // 4. 显示各个玩家的头像
            Player* player = _playerList.at(i);
            QPixmap pixmap = loadRoleImage(player->getSex(), player->getDirection(), player->getRole());
            context.roleImg->setPixmap(pixmap);
            context.roleImg->show();
        }
        break;
    }
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
        break;
    }
    case GameControl::ThinkingForPlayHand:
    {
        hidePlayerDropCards(player); // 隐藏上一轮打出的牌
        if(player == _gameControl->getUser()) {
            Player* pendPlayer = _gameControl->getPendPlayer(); // 获取出牌玩家
            if(pendPlayer == _gameControl->getUser() || pendPlayer == nullptr) {
                ui->btnGroup->selectPage(ButtonGroup::PlayCard);
            }
            else {
                ui->btnGroup->selectPage(ButtonGroup::PassOrPlay);
            }
        }
        else { ui->btnGroup->selectPage(ButtonGroup::Empty); }
        break;
    }
    case GameControl::Winning:
    {
        // 背景音乐停止播放
        _bgm->stopBGM();
        // 显示左右机器人玩家的扑克牌
        _contextMap[_gameControl->getLeftRobot()].isFrontSide = true;
        _contextMap[_gameControl->getRightRobot()].isFrontSide = true;
        updatePlayerCards(_gameControl->getLeftRobot());
        updatePlayerCards(_gameControl->getRightRobot());
        // 更新玩家的得分
        updatePlayerScore();
        // 本局获胜的玩家，可在下局游戏中优先抢地主
        _gameControl->setCurrPlayer(player);
        showEndingScorePanel(); // 游戏结束时，显示分数面板窗口
        break;
    }
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
    _bgm->playAssintMusic(BGMControl::Dispatch);
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
        // Cards cards(card);
        // disposCard(curPlayer, cards);
        // 切换玩家
        _gameControl->setCurrPlayer(curPlayer->getNextPlayer());
        curMovePos = 0;
        cardMoveStep(curPlayer, curMovePos); // 发牌动画
        // 判断牌是否发完了
        if(_gameControl->getSurplusCards().cardCount() == 3) {
            _timer->stop(); // 终止定时器
            gameStatusPrecess(GameControl::CallingLord); // 切换游戏状态
            // 终止发牌音乐的播放
            _bgm->stopAssintMusic();
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
    const int unit[] = {
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
    _cardsRect = QRect(); // 初始化_cardsRect
    // 取出展示扑克牌的区域
    int cardSpace = 20; // 显示的卡牌之间的间隙
    _userCards.clear(); // 清空用户手牌的卡片窗口的旧位置(∵已经不适用了)
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
            _cardsRect = QRect(leftX, topY, cardSpace * i + _cardSize.width(), _cardSize.height()); // 更新_cardsRect
            // 判断当前手牌的卡牌窗口在主窗口的显示宽度是cardSpace还是完整卡牌窗口的宽度
            int curWidth = (i != list.size() - 1) ? cardSpace : _cardSize.width();
            // 获取当前手牌的卡牌窗口在主窗口中的显示位置
            QRect cardRect(leftX + cardSpace * i, topY, curWidth, _cardSize.height());
            _userCards.insert(panel, cardRect);
        }
        else {
            int leftX = cardsRect.left() + (cardsRect.width() - _cardSize.width()) / 2;
            int topY = cardsRect.top() + (cardsRect.height() - (list.size() - 1) * cardSpace - panel->height()) / 2;
            panel->move(leftX, topY + cardSpace * i);
        }
    }
    // 显示玩家打出的牌
    // 得到当前玩家的出牌区域以及本轮打出的牌
    QRect playCardRect = _contextMap[player].playCardRect;
    Cards lastCards = _contextMap[player].lastCards;
    if( !lastCards.isEmpty() ) { // 若本轮打出的牌不为空, 则显示本轮打出的牌
        int playSpacing = 24;    // 显示的扑克牌之间的间隙
        CardList lastCardList = lastCards.toCardList();
        CardList::ConstIterator itplayed = lastCardList.constBegin();
        for(int i = 0; itplayed != lastCardList.constEnd(); ++itplayed, ++i) {
            CardPanel* panel = _cardMap[*itplayed]; // 获取本轮打出的牌的卡牌窗口
            panel->setFrontSide(true);              // 显示正面
            panel->raise();
            // 将打出的牌移动到出牌区域
            if(_contextMap[player].align == Horizontal) { // 显示方式为水平显示
                int leftBase = playCardRect.left() +
                               (playCardRect.width() - (lastCardList.size() - 1) * playSpacing - panel->width()) / 2;
                int top = playCardRect.top() + (playCardRect.height() - panel->height()) / 2;
                panel->move(leftBase + i * playSpacing, top);
            }
            else { // 显示方式为垂直显示
                int left = playCardRect.left() + (playCardRect.width() - panel->width()) / 2;
                int top = playCardRect.top();
                panel->move(left, top + i * playSpacing);
            }
            panel->show();
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
        // 显示抢地主的分数
        showAnimation(Bet, bet);
    }
    context.info->show();

    // 播放分数的背景音乐
    _bgm->playerRobLordMusic(bet, (BGMControl::RoleSex)player->getSex(), flag);
}

void GamePanel::onDisposePlayHand(Player *player, Cards &cards) {
    // 1. 存储玩家打出的牌
    auto it = _contextMap.find(player);
    it->lastCards = cards;

    // 2. 根据牌型播放游戏特效
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getHandType();
    if(type == PlayHand::Hand_Plane || type == PlayHand::Hand_Plane_Two_Pair ||
        type == PlayHand::Hand_Plane_Two_Single)
    {
        showAnimation(Plane);
    }
    else if(type == PlayHand::Hand_Seq_Pair) {
        showAnimation(LianDui);
    }
    else if(type == PlayHand::Hand_Seq_Single) {
        showAnimation(ShunZi);
    }
    else if(type == PlayHand::Hand_Bomb) {
        showAnimation(Bomb);
    }
    else if(type == PlayHand::Hand_Bomb_Jokers) {
        showAnimation(JokerBomb);
    }
    // 若玩家打出的是空牌(不出牌), 则显示提示信息
    if(cards.isEmpty()) {
        it->info->setPixmap(QPixmap(":/images/pass.png"));
        it->info->show();
        _bgm->playPassMusic((BGMControl::RoleSex)player->getSex());
    }
    else {
        if(_gameControl->getPendPlayer() == player || _gameControl->getPendPlayer() == nullptr) { // 是第一个出牌
            _bgm->playCardMusic(cards, true, (BGMControl::RoleSex)player->getSex());
        }
        else { // 不是第一个出牌
            _bgm->playCardMusic(cards, false, (BGMControl::RoleSex)player->getSex());
        }
    }

    // 3. 更新玩家剩余的牌
    updatePlayerCards(player);

    // 4. 播放提示音效
    // 判断玩家剩余的牌的数量
    if(player->getCards().cardCount() == 2) {
        _bgm->playLastMusic(BGMControl::Last2, (BGMControl::RoleSex)player->getSex());
    }
    else if(player->getCards().cardCount() == 1) {
        _bgm->playLastMusic(BGMControl::Last1, (BGMControl::RoleSex)player->getSex());
    }
}

void GamePanel::hidePlayerDropCards(Player *player) {
    auto it = _contextMap.find(player); // 获取[(玩家player及其手牌)在主窗口的上下文信息对象]的迭代器
    if(it != _contextMap.end()) {     // 若it有效
        if(it->lastCards.isEmpty()) { // 用户上次没有出牌, 即显示不出
            it->info->hide(); // 隐藏"不出"
        }
        else { // 用户上次出牌了
            CardList list = it->lastCards.toCardList(); // 获取上一次打出的牌
            // 隐藏上一次打出的牌
            for(auto last = list.begin(); last != list.end(); ++last) {
                _cardMap[*last]->hide();
            }
        }
        it->lastCards.clear();
    }
}

void GamePanel::onCardSelected(Qt::MouseButton button) {
    // 1. 判断是不是出牌状态
    if(_gameStatus == GameControl::CallingLord || _gameStatus == GameControl::Dispatch) {
        return; // 若游戏状态不为出牌, 则不作任何处理
    }
    // 2. 判断发出信号的牌的所有者是不是当前用户玩家
    CardPanel* panel = static_cast<CardPanel*>(sender());
    if(panel->getOwner() != _gameControl->getUser()) {
        return;
    }
    // 3. 保存当前被选中的牌的窗口对象
    _curSelCard = panel;
    // 4. 判断参数button对应的鼠标键是左键还是右键
    if(button == Qt::LeftButton) { // 鼠标左键点击卡牌窗口
        // 设置卡牌窗口的选中状态: 将卡牌窗口的选中状态进行"取反"
        panel->setSelected(!panel->isSelected());
        // 更新卡牌窗口在主窗口中的显示
        updatePlayerCards(panel->getOwner());
        // (保存or删除)卡牌窗口对象到_selectCards: 已存在就删除, 不存在就添加
        QSet<CardPanel*>::const_iterator it = _selectCards.find(panel);
        if(it == _selectCards.constEnd()) { // 当前选中的卡牌窗口不在_selectCards中: 则添加
            _selectCards.insert(panel);
        }
        else { // 当前选中的卡牌窗口已存在_selectCards中: 则添加
            _selectCards.erase(it);
        }
        _bgm->playAssintMusic(BGMControl::SelectCard);
    }
    else if(button == Qt::RightButton) { // 鼠标右键点击卡牌窗口 → 调用出牌按钮的槽函数
        onUserPlayHand();
    }
}

void GamePanel::onUserPlayHand() {
    // 判断游戏状态: 不是出牌状态则直接返回
    if(_gameStatus != GameControl::PlayingHand) { return; }

    // 判断调用者是不是用户玩家: 不是则直接返回
    if(_gameControl->getCurrPlayer() != _gameControl->getUser()) { return; }

    // 判断要出的牌是否为空: 等价于判断_selectCards是否为空, 为空则直接返回
    if(_selectCards.isEmpty()) { return; }

    // 得到要打出的牌的牌型
    Cards playCards; // 存储要打出的牌
    for(auto it = _selectCards.begin(); it != _selectCards.end(); ++it) {
        Card card = (*it)->getCard();
        playCards.add(card);
    }
    PlayHand hand(playCards);                     // 获取要打出的牌的牌型对象hand
    PlayHand::HandType type = hand.getHandType(); // type为打出的牌的牌型
    if(type == PlayHand::Hand_Unknow) { return; }

    // 判断当前玩家的牌能不能压住[已出牌玩家打出的牌]
    if(_gameControl->getPendPlayer() != _gameControl->getUser()) { // 已出牌玩家不是用户玩家
        Cards cards = _gameControl->getPendCards();
        if( !hand.canBeat(PlayHand(cards)) ) { // 若[要打出的牌]不大于[待处理的牌]
            return;
        }
    }
    // 执行到此处, 则[要打出的牌]大于[待处理的牌]

    _countDown->stopCountDown(); // 停止出牌倒计时
    // 让玩家对象出牌
    _gameControl->getUser()->playHand(playCards);
    // 打出牌后，清空[存储要打出的牌的容器]
    _selectCards.clear();
}

void GamePanel::onUserPass() {
    _countDown->stopCountDown(); // 停止出牌倒计时

    // 判断是不是用户玩家
    Player* curPlayer = _gameControl->getCurrPlayer();
    Player* userPlayer = _gameControl->getUser();
    if(curPlayer != userPlayer) { return; }

    // 判断上一次出牌玩家是不是[当前玩家/空](可以不处理): 若是则直接返回
    Player* pendPlayer = _gameControl->getPendPlayer();
    if(pendPlayer == userPlayer || pendPlayer == nullptr) { return; }

    // 打出一个空的Cards对象
    Cards empty;
    userPlayer->playHand(empty);

    // 清空用户选择的牌(玩家可能选择了一些牌, 但是没有打出去)
    for(auto it = _selectCards.begin(); it != _selectCards.end(); ++it) {
        (*it)->setSelected(false);
    }
    _selectCards.clear();
    // 更新玩家待出牌区域的牌
    updatePlayerCards(userPlayer);
}

void GamePanel::showAnimation(AnimationType type, int bet) {
    switch(type) {
    case AnimationType::LianDui:
    case AnimationType::ShunZi:
    {
        _animation->setFixedSize(250, 150);
        _animation->move((width() - _animation->width()) / 2, 200);
        _animation->showSequence((AnimationWindow::SeqType)type);
        break;
    }
    case AnimationType::Plane:
    {
        _animation->setFixedSize(800, 75);
        _animation->move((width() - _animation->width()) / 2, 200);
        _animation->showPlane();
        break;
    }
    case AnimationType::Bomb:
    {
        _animation->setFixedSize(180, 200);
        _animation->move((width() - _animation->width()) / 2, (height() - _animation->height()) / 2 - 70);
        _animation->showBomb();
        break;
    }
    case AnimationType::JokerBomb:
    {
        _animation->setFixedSize(250, 200);
        _animation->move((width() - _animation->width()) / 2, (height() - _animation->height()) / 2 - 70);
        _animation->showJokerBomb();
        break;
    }
    case AnimationType::Bet:
    {
        _animation->setFixedSize(160, 98);
        _animation->move((width() - _animation->width()) / 2, (height() - _animation->height()) / 2 - 140);
        _animation->showBetScore(bet);
        break;
    }
    }
    _animation->show(); // 还需要显示其他特效动画
}

QPixmap GamePanel::loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role) {
    // 找图片
    QVector<QString> lordMan;
    QVector<QString> lordWoman;
    QVector<QString> farmerMan;
    QVector<QString> farmerWoman;
    lordMan << ":/images/lord_man_1.png" << ":/images/lord_man_2.png";
    lordWoman << ":/images/lord_woman_1.png" << ":/images/lord_woman_2.png";
    farmerMan << ":/images/farmer_man_1.png" << ":/images/farmer_man_2.png";
    farmerWoman << ":/images/farmer_woman_1.png" << ":/images/farmer_woman_2.png";
    // 加载图片: 使用QImage, ∵QImage可以使用镜像功能，就能支持头像(朝左/朝右)
    QImage image;
    int random = QRandomGenerator::global()->bounded(2);
    if(sex == Player::Man && role == Player::Lord) {
        image.load(lordMan.at(random));
    }
    else if(sex == Player::Man && role == Player::Farmer) {
        image.load(farmerMan.at(random));
    }
    else if(sex == Player::Woman && role == Player::Lord) {
        image.load(lordWoman.at(random));
    }
    else if(sex == Player::Woman && role == Player::Farmer) {
        image.load(farmerWoman.at(random));
    }
    QPixmap pixmap;
    if(direct == Player::Left) {
        pixmap = QPixmap::fromImage(image.mirrored(true, false));
    }
    else {
        pixmap = QPixmap::fromImage(image);
    }
    return pixmap;
}

void GamePanel::showEndingScorePanel() {
    bool isLord = (_gameControl->getUser()->getRole() == Player::Lord) ? true : false;
    bool isWin = _gameControl->getUser()->isWin();
    EndingPanel* panel = new EndingPanel(isLord, isWin, this);
    panel->show();
    // panel->move((width() - panel->width()) / 2, (height() - panel->height()) / 2);
    panel->setPlayerScore(_gameControl->getLeftRobot()->getScore(),  // 设置分数面板上个玩家的分数
                          _gameControl->getRightRobot()->getScore(),
                          _gameControl->getUser()->getScore());

    _bgm->playEndingMusic(isWin);

    QPropertyAnimation* animation = new QPropertyAnimation(panel, "geometry", this); // 创建分数面板的属性动画对象
    animation->setDuration(2500); // 动画持续的时间: 2.5s
    // 设置窗口的起始位置和终止位置
    animation->setStartValue(QRect(panel->x(), panel->y(), panel->width(), panel->height()));
    animation->setEndValue(QRect((width() - panel->width()) / 2, (height() - panel->height()) / 2,
                                 panel->width(), panel->height()));
    // 设置窗口的运动曲线
    animation->setEasingCurve(QEasingCurve(QEasingCurve::OutBounce));
    // 播放动画
    animation->start();
    // 处理按钮的信号
    connect(panel, &EndingPanel::continueGame, this, [=](){
        panel->close();
        panel->deleteLater();
        animation->deleteLater();
        ui->btnGroup->selectPage(ButtonGroup::Empty); // 按钮组显示空界面
        gameStatusPrecess(GameControl::Dispatch);     // 将游戏状态设置为发牌状态
        _bgm->startBGM(80);
    });
}

void GamePanel::initCountDown() {
    _countDown = new CountDown(this);
    _countDown->move((width() - _countDown->width()) / 2, (height() - _countDown->height()) / 2 + 30);
    connect(_countDown, &CountDown::notMuchTime, this, [=](){
        // 没有多少的时间用于思考出牌了, 现在要播放提示音乐
        _bgm->playAssintMusic(BGMControl::Alert);
    });
    // 倒计时结束, 用户被剥夺出牌权
    connect(_countDown, &CountDown::timeout, this, &GamePanel::onUserPass);
    User* user = _gameControl->getUser();
    connect(user, &User::startCountDown, this, [=](){
        if(_gameControl->getPendPlayer() != user && _gameControl->getPendPlayer() != nullptr) {
            _countDown->showCountDown();
        }
    });
}

void GamePanel::paintEvent(QPaintEvent *pe) {
    Q_UNUSED(pe);
    QPainter p(this);
    p.drawPixmap(rect(), _bkImage);
}

void GamePanel::mouseMoveEvent(QMouseEvent *me) {
    if(me->buttons() & Qt::LeftButton) {  // 若鼠标按下的键包含鼠标左键
        QPoint point = me->pos();         // 获取鼠标的当前坐标点
        if(!_cardsRect.contains(point)) { // 若[玩家的待出牌区域]没有包含[鼠标的当前坐标点]
            _curSelCard = nullptr;
        }
        else { // 若[玩家的待出牌区域]包含了[鼠标的当前坐标点]
            QList<CardPanel*> list = _userCards.keys(); // 获取[存储着<CardPanel*, QRect>的_userCards]的所有键值
            for(int i = 0; i < list.size(); ++i) {
                CardPanel* panel = list[i]; // 遍历获取剩余手牌中的某一卡牌窗口
                if(_userCards[panel].contains(point) && _curSelCard != panel) {
                    // 若当前卡牌窗口在主窗口中的显示区域包含了[鼠标的当前坐标点]
                    panel->clicked(); // 点击这张卡牌
                    _curSelCard = panel;
                }
            }
        }
    }
}
