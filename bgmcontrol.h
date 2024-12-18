#ifndef BGMCONTROL_H
#define BGMCONTROL_H

#include <QObject>
#include <QMediaPlayer>
#include "mediaplaylist.h"
#include <QAudioOutput>
#include "cards.h"
#include "playhand.h"

class BGMControl : public QObject {
    Q_OBJECT
public:
    enum RoleSex { Man, Woman };

    enum CardType {
        // 单张拍
        Three, Foue, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace, Tow,
        SmallJoker, BigJoker,
        // 对子
        Three_Double, Foue_Double, Five_Double, Six_Double, Seven_Double, Eight_Double, Nine_Double,
        Ten_Double, Jack_Double, Queen_Double, King_Double, Ace_Double, Tow_Double,
        // 三张同点数的牌
        Three_Triple, Foue_Triple, Five_Triple, Six_Triple, Seven_Triple, Eight_Triple, Nine_Triple,
        Ten_Triple, Jack_Triple, Queen_Triple, King_Triple, Ace_Triple, Tow_Triple,
        Plane,          // 飞机
        SequencePair,   // 连对
        ThreeBindOne,   // 三带一
        ThreeBindPair,  // 三带一对
        Sequence,       // 顺子
        FourBindTwo,    // 四带二(单张)
        FourBind2Pair,  // 四带两对
        Bomb,           // 炸弹
        JokerBomb,      // 王炸
        Pass1, Pass2, Pass3, Pass4, // 过
        MoreBiger1, MoreBiger2,     // 大你
        Biggest,                    // 压死
        // 抢地主
        NoOrder,        // 不叫
        NoRob,          // 不抢
        Order,          // 叫地主
        Rob1, Rob2,     // 抢地主
        Last1,          // 只剩1张牌
        Last2           // 只剩2张牌
    };

    enum AssistMusic {
        Dispatch,   // 发牌
        SelectCard, // 选牌
        PlaneVoice, // 飞机
        BombVoice,  // 炸弹
        Alert,      // 提醒
    };

    explicit BGMControl(QObject *parent = nullptr);
    void initMediaList(); // 初始化播放列表
    // 背景音乐(循环播放)
    void startBGM(int volume); // volume为音量大小
    void stopBGM();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus state);

    // 播放玩家抢地主音乐
    void playerRobLordMusic(int point, RoleSex sex, bool isFirst);

    // 播放玩家出牌音乐
    void playCardMusic(Cards cards, bool isFirst, RoleSex sex);
    void playLastMusic(CardType type, RoleSex sex);

    // 播放不出牌的背景音乐
    void playPassMusic(RoleSex sex);

    // 播放辅助音乐
    void playAssintMusic(AssistMusic type);
    void stopAssintMusic();

    // 播放结尾音乐
    void playEndingMusic(bool isWin);
signals:
private:
    // 需要播放的音乐: man、woman、bgm、辅助音乐、结束音乐
    //         索引:  0     1     2     3       4
    QVector<QMediaPlayer*> _mediaPlayer;  // 媒体播放器
    QVector<MediaPlayList*> _mediaList;   // 音频列表
};

#endif // BGMCONTROL_H
