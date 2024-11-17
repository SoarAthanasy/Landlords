#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>

namespace Ui { class ScorePanel; }

class ScorePanel : public QWidget { // 游戏分数面板窗口
    Q_OBJECT
public:
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();
private:
    Ui::ScorePanel *ui;
};

#endif // SCOREPANEL_H
