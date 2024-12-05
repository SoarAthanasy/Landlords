#include "endingpanel.h"
#include <QPainter>

EndingPanel::EndingPanel(bool isLord, bool isWin, QWidget *parent) : QWidget{parent} {
    // 加载背景图片
    _backImage.load(":/images/gameover.png");
    setFixedSize(_backImage.size());

    // 显示用户玩家的角色以及游戏状态
    _title = new QLabel(this);
    if(isLord && isWin) {
        _title->setPixmap(QPixmap(":/images/lord_win.png"));
    }
    else if(isLord && !isWin) {
        _title->setPixmap(QPixmap(":/images/lord_fail.png"));
    }
    else if(!isLord && isWin) {
        _title->setPixmap(QPixmap(":/images/farmer_win.png"));
    }
    else if(!isLord && !isWin) {
        _title->setPixmap(QPixmap(":/images/farmer_fail.png"));
    }
    _title->move(125, 125);

    // 初始化分数面板
    _scorePanel = new ScorePanel(this);
    _scorePanel->move(75, 230);
    _scorePanel->setFixedSize(260, 160);
    _scorePanel->setFontColor(ScorePanel::Red);
    _scorePanel->setFontSize(18);

    // 按钮: 继续游戏
    _continue = new QPushButton(this);
    _continue->move(84, 429);
    QString style = R"(
        QPushButton{border-image: url(:/images/button_normal.png)}
        QPushButton:hover{border-image: url(:/images/button_hover.png)}
        QPushButton:pressed{border-image: url(:/images/button_pressed.png)}
    )";
    _continue->setStyleSheet(style);
    _continue->setFixedSize(231, 48);
    connect(_continue, &QPushButton::clicked, this, &EndingPanel::continueGame);
}

void EndingPanel::setPlayerScore(int left, int right, int user) {
    _scorePanel->setScores(left, right, user);
}

void EndingPanel::paintEvent(QPaintEvent *pe) {
    QPainter p(this);
    p.drawPixmap(rect(), _backImage);
}
