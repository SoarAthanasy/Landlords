#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>

namespace Ui { class ScorePanel; }

class ScorePanel : public QWidget { // 游戏分数面板窗口
    Q_OBJECT
public:
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();

    void setScores(int left, int right, int user); // 设置玩家的得分
private:
    Ui::ScorePanel *ui;
};

#endif // SCOREPANEL_H
