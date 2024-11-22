#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui { class ButtonGroup; }

class ButtonGroup : public QWidget { // 游戏窗口中的按钮组窗口
    Q_OBJECT
public:
    // 各显示界面的索引
    enum Panel { Start, PlayCard, PassOrPlay, CallLord, Empty };

    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();

    void initButtons(); // 初始化按钮组
    void selectPage(Panel type, int bet = 0);  // 处理page页的切换
signals:
    void startGame();       // 开始游戏
    void playHand();        // 出牌
    void pass();            // 不出牌
    void betPoint(int bet); // 抢地主
private:
    Ui::ButtonGroup *ui;
};

#endif // BUTTONGROUP_H
