/********************************************************************************
** Form generated from reading UI file 'buttongroup.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUTTONGROUP_H
#define UI_BUTTONGROUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include "mybutton.h"

QT_BEGIN_NAMESPACE

class Ui_ButtonGroup
{
public:
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *startPage;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    MyButton *start;
    QSpacerItem *horizontalSpacer_2;
    QWidget *playCardPage;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    MyButton *playCard;
    QSpacerItem *horizontalSpacer_4;
    QWidget *playOrPassPage;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    MyButton *play;
    QSpacerItem *horizontalSpacer_6;
    MyButton *Pass;
    QSpacerItem *horizontalSpacer_7;
    QWidget *callLordPage;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    MyButton *giveup;
    QSpacerItem *horizontalSpacer_10;
    MyButton *oneScore;
    QSpacerItem *horizontalSpacer_11;
    MyButton *twoScore;
    QSpacerItem *horizontalSpacer_12;
    MyButton *threeScore;
    QSpacerItem *horizontalSpacer_9;
    QWidget *nullPage;

    void setupUi(QWidget *ButtonGroup)
    {
        if (ButtonGroup->objectName().isEmpty())
            ButtonGroup->setObjectName("ButtonGroup");
        ButtonGroup->resize(611, 145);
        horizontalLayout = new QHBoxLayout(ButtonGroup);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(ButtonGroup);
        stackedWidget->setObjectName("stackedWidget");
        startPage = new QWidget();
        startPage->setObjectName("startPage");
        horizontalLayout_2 = new QHBoxLayout(startPage);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(245, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        start = new MyButton(startPage);
        start->setObjectName("start");

        horizontalLayout_2->addWidget(start);

        horizontalSpacer_2 = new QSpacerItem(244, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        stackedWidget->addWidget(startPage);
        playCardPage = new QWidget();
        playCardPage->setObjectName("playCardPage");
        horizontalLayout_3 = new QHBoxLayout(playCardPage);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_3 = new QSpacerItem(245, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        playCard = new MyButton(playCardPage);
        playCard->setObjectName("playCard");

        horizontalLayout_3->addWidget(playCard);

        horizontalSpacer_4 = new QSpacerItem(244, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        stackedWidget->addWidget(playCardPage);
        playOrPassPage = new QWidget();
        playOrPassPage->setObjectName("playOrPassPage");
        horizontalLayout_4 = new QHBoxLayout(playOrPassPage);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_5 = new QSpacerItem(176, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        play = new MyButton(playOrPassPage);
        play->setObjectName("play");

        horizontalLayout_4->addWidget(play);

        horizontalSpacer_6 = new QSpacerItem(40, 15, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        Pass = new MyButton(playOrPassPage);
        Pass->setObjectName("Pass");

        horizontalLayout_4->addWidget(Pass);

        horizontalSpacer_7 = new QSpacerItem(176, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        stackedWidget->addWidget(playOrPassPage);
        callLordPage = new QWidget();
        callLordPage->setObjectName("callLordPage");
        horizontalLayout_5 = new QHBoxLayout(callLordPage);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        giveup = new MyButton(callLordPage);
        giveup->setObjectName("giveup");

        horizontalLayout_5->addWidget(giveup);

        horizontalSpacer_10 = new QSpacerItem(40, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_10);

        oneScore = new MyButton(callLordPage);
        oneScore->setObjectName("oneScore");

        horizontalLayout_5->addWidget(oneScore);

        horizontalSpacer_11 = new QSpacerItem(40, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_11);

        twoScore = new MyButton(callLordPage);
        twoScore->setObjectName("twoScore");

        horizontalLayout_5->addWidget(twoScore);

        horizontalSpacer_12 = new QSpacerItem(40, 10, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_12);

        threeScore = new MyButton(callLordPage);
        threeScore->setObjectName("threeScore");

        horizontalLayout_5->addWidget(threeScore);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_9);

        stackedWidget->addWidget(callLordPage);
        nullPage = new QWidget();
        nullPage->setObjectName("nullPage");
        stackedWidget->addWidget(nullPage);

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(ButtonGroup);

        QMetaObject::connectSlotsByName(ButtonGroup);
    } // setupUi

    void retranslateUi(QWidget *ButtonGroup)
    {
        ButtonGroup->setWindowTitle(QCoreApplication::translate("ButtonGroup", "Form", nullptr));
        start->setText(QCoreApplication::translate("ButtonGroup", "\345\274\200\345\247\213", nullptr));
        playCard->setText(QCoreApplication::translate("ButtonGroup", "\345\207\272\347\211\214", nullptr));
        play->setText(QCoreApplication::translate("ButtonGroup", "\345\207\272\347\211\214", nullptr));
        Pass->setText(QCoreApplication::translate("ButtonGroup", "\350\267\263\350\277\207", nullptr));
        giveup->setText(QCoreApplication::translate("ButtonGroup", "\346\224\276\345\274\203", nullptr));
        oneScore->setText(QCoreApplication::translate("ButtonGroup", "1\345\210\206", nullptr));
        twoScore->setText(QCoreApplication::translate("ButtonGroup", "2\345\210\206", nullptr));
        threeScore->setText(QCoreApplication::translate("ButtonGroup", "3\345\210\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ButtonGroup: public Ui_ButtonGroup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUTTONGROUP_H
