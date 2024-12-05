#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H

#include <QWidget>
#include <QLabel>
#include "scorepanel.h"
#include <QPushButton>

class EndingPanel : public QWidget {
    Q_OBJECT
public:
    explicit EndingPanel(bool isLord, bool isWin, QWidget *parent = nullptr);
    void setPlayerScore(int left, int right, int user);
signals:
    void continueGame();
protected: // 事件处理器------------------------------
    void paintEvent(QPaintEvent* pe); // 窗口重绘事件
private:
    QPixmap _backImage;      // 背景图片
    QLabel* _title;          // 显示用户玩家的角色和输赢
    ScorePanel* _scorePanel; // 分数面板
    QPushButton* _continue;  // 按钮: 继续游戏
};

#endif // ENDINGPANEL_H
