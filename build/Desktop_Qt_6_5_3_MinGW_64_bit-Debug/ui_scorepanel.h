/********************************************************************************
** Form generated from reading UI file 'scorepanel.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCOREPANEL_H
#define UI_SCOREPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScorePanel
{
public:
    QGridLayout *gridLayout;
    QLabel *me;
    QLabel *myScore;
    QLabel *meUnit;
    QLabel *leftRobot;
    QLabel *leftScore;
    QLabel *leftUnit;
    QLabel *rightRobot;
    QLabel *rightScore;
    QLabel *rightUnit;

    void setupUi(QWidget *ScorePanel)
    {
        if (ScorePanel->objectName().isEmpty())
            ScorePanel->setObjectName("ScorePanel");
        ScorePanel->resize(230, 148);
        gridLayout = new QGridLayout(ScorePanel);
        gridLayout->setObjectName("gridLayout");
        me = new QLabel(ScorePanel);
        me->setObjectName("me");
        QFont font;
        font.setFamilies({QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221")});
        font.setPointSize(12);
        font.setBold(true);
        me->setFont(font);
        me->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        me->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(me, 0, 0, 1, 1);

        myScore = new QLabel(ScorePanel);
        myScore->setObjectName("myScore");
        myScore->setFont(font);
        myScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        myScore->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(myScore, 0, 1, 1, 1);

        meUnit = new QLabel(ScorePanel);
        meUnit->setObjectName("meUnit");
        meUnit->setFont(font);
        meUnit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        meUnit->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(meUnit, 0, 2, 1, 1);

        leftRobot = new QLabel(ScorePanel);
        leftRobot->setObjectName("leftRobot");
        leftRobot->setFont(font);
        leftRobot->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        leftRobot->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(leftRobot, 1, 0, 1, 1);

        leftScore = new QLabel(ScorePanel);
        leftScore->setObjectName("leftScore");
        leftScore->setFont(font);
        leftScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        leftScore->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(leftScore, 1, 1, 1, 1);

        leftUnit = new QLabel(ScorePanel);
        leftUnit->setObjectName("leftUnit");
        leftUnit->setFont(font);
        leftUnit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        leftUnit->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(leftUnit, 1, 2, 1, 1);

        rightRobot = new QLabel(ScorePanel);
        rightRobot->setObjectName("rightRobot");
        rightRobot->setFont(font);
        rightRobot->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        rightRobot->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(rightRobot, 2, 0, 1, 1);

        rightScore = new QLabel(ScorePanel);
        rightScore->setObjectName("rightScore");
        rightScore->setFont(font);
        rightScore->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        rightScore->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(rightScore, 2, 1, 1, 1);

        rightUnit = new QLabel(ScorePanel);
        rightUnit->setObjectName("rightUnit");
        rightUnit->setFont(font);
        rightUnit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        rightUnit->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(rightUnit, 2, 2, 1, 1);


        retranslateUi(ScorePanel);

        QMetaObject::connectSlotsByName(ScorePanel);
    } // setupUi

    void retranslateUi(QWidget *ScorePanel)
    {
        ScorePanel->setWindowTitle(QCoreApplication::translate("ScorePanel", "Form", nullptr));
        me->setText(QCoreApplication::translate("ScorePanel", "\346\210\221", nullptr));
        myScore->setText(QCoreApplication::translate("ScorePanel", "0", nullptr));
        meUnit->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
        leftRobot->setText(QCoreApplication::translate("ScorePanel", "\345\267\246\344\276\247\346\234\272\345\231\250\344\272\272", nullptr));
        leftScore->setText(QCoreApplication::translate("ScorePanel", "0", nullptr));
        leftUnit->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
        rightRobot->setText(QCoreApplication::translate("ScorePanel", "\345\217\263\344\276\247\346\234\272\345\231\250\344\272\272", nullptr));
        rightScore->setText(QCoreApplication::translate("ScorePanel", "0", nullptr));
        rightUnit->setText(QCoreApplication::translate("ScorePanel", "\345\210\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScorePanel: public Ui_ScorePanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCOREPANEL_H
