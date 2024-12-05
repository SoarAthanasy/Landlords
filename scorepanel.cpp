#include "scorepanel.h"
#include "ui_scorepanel.h"

ScorePanel::ScorePanel(QWidget *parent) : QWidget(parent), ui(new Ui::ScorePanel) {
    ui->setupUi(this);
    _list << ui->me << ui->myScore << ui->meUnit
          << ui->leftRobot << ui->leftScore << ui->leftUnit
          << ui->rightRobot << ui->rightScore << ui->rightUnit;
}

ScorePanel::~ScorePanel() { delete ui; }

void ScorePanel::setScores(int left, int right, int user) {
    ui->leftScore->setText(QString::number(left));
    ui->rightScore->setText(QString::number(right));
    ui->myScore->setText(QString::number(user));
}

void ScorePanel::setFontSize(int point) {
    QFont font("微软雅黑", point, QFont::Bold);
    for(int i = 0; i < _list.size(); ++i) {
        _list[i]->setFont(font);
    }
}

const QString MYCOLOR[] = {"black", "white", "red", "blue", "green"};
void ScorePanel::setFontColor(FontColor color) {
    QString style = QString("QLabel{color:%1}").arg(MYCOLOR[color]);
    for(int i = 0; i < _list.size(); ++i) {
        _list[i]->setStyleSheet(style);
    }
}
