#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QLabel>

namespace Ui { class ScorePanel; }

class ScorePanel : public QWidget { // 游戏分数面板窗口
    Q_OBJECT
public:
    enum FontColor { Black, White, Red, Blue, Green };

    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();

    void setScores(int left, int right, int user); // 设置玩家的得分
    void setFontSize(int point);                   // 设置字体大小
    void setFontColor(FontColor color);            // 设置字体颜色
private:
    Ui::ScorePanel *ui;
    QVector<QLabel*> _list; // 存储UI控件的地址
};

#endif // SCOREPANEL_H
