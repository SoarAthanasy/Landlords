#include "bgmcontrol.h"
#include "playhand.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

BGMControl::BGMControl(QObject *parent) : QObject{parent} {
    for(int i = 0; i < 5; ++i) {
        QMediaPlayer* mediaPlayer = new QMediaPlayer(this); // 实例化媒体播放器
        QAudioOutput* audioOutput = new QAudioOutput(this); // 实例化音频输出组件
        MediaPlayList* mediaList = new MediaPlayList(this); // 实例化媒体播放列表
        mediaPlayer->setAudioOutput(audioOutput); // 关联音频输出
        // if(i == 2) { mediaPlayer->setLoops(QMediaPlayer::Infinite); } // BGM无限循环播放
        audioOutput->setVolume(100);              // 设置音量
        _mediaPlayer.push_back(mediaPlayer);
        _mediaList.push_back(mediaList);
    }
    initMediaList();

    connect(_mediaPlayer[2], &QMediaPlayer::mediaStatusChanged, this, &BGMControl::onMediaStatusChanged);
}

void BGMControl::initMediaList() {
    qDebug() << "开始调用BGMControl::initMediaList()";
    QStringList list;
    list << "Man" << "Woman" << "BGM" << "Other" << "Ending";

    // 1. 读Json配置文件
    QFile file(":/conf/playList.json");
    file.open(QFile::ReadOnly);       // 以只读方式打开
    QByteArray json = file.readAll(); // 读取Json文件的所有数据
    file.close();

    // 2. 解析从Json中读出的Json数据
        // 将读取到的QByteArray形式的json, 转化为Json文档形式的doc
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();

    // 3. 初始化多媒体播放列表
    for(int i = 0; i < list.size(); ++i) {
        QString prefix = list[i];
        QJsonArray array = obj.value(prefix).toArray(); // 获取list[i]对应的Json数组
        for(int j = 0; j < array.size(); ++j) {
            _mediaList[i]->addMedia(QUrl(array[j].toString()));
        }
    }
    qDebug() << "_mediaList各元素的大小: " << _mediaList[0]->size() << _mediaList[1]->size()
             << _mediaList[2]->size() << _mediaList[3]->size() << _mediaList[4]->size();
    qDebug() << "调用BGMControl::initMediaList()成功";
}

void BGMControl::startBGM(int volume) {
    qDebug() << "开始调用BGMControl::startBGM()";
    MediaPlayList* BGMList = _mediaList[2];        // 获取BGM对应的媒体列表
    QMediaPlayer* mediaPlayer = _mediaPlayer[2];   // 获取BGM对应的媒体播放器
    BGMList->setCurrIndex(0);                      // 播放第一首BGM
    mediaPlayer->setSource(BGMList->currMedia());  // 播放媒体列表的当前媒体
    mediaPlayer->audioOutput()->setVolume(volume); // 调整播放器的声音大小
    mediaPlayer->play();                           // 开始播放
    qDebug() << "调用BGMControl::startBGM()成功";
}

void BGMControl::stopBGM() { _mediaPlayer[2]->stop(); }

void BGMControl::onMediaStatusChanged(QMediaPlayer::MediaStatus state) {
    if(state == QMediaPlayer::EndOfMedia) {
        _mediaList[2]->setCurrIndex( (_mediaList[2]->getCurrIndex() + 1) % 4);
        _mediaPlayer[2]->setSource(_mediaList[2]->currMedia());
        _mediaPlayer[2]->play();
    }
}

void BGMControl::playerRobLordMusic(int point, RoleSex sex, bool isFirst) {
    qDebug() << "开始调用BGMControl::playerRobLordMusic()";
    // 玩家下注了没有?
    // 玩家的性别
    // 是否为第一个抢地主的
    int index = (sex == Man) ? 0 : 1; // 获取性别sex对应的音频列表的索引
    if(isFirst && point > 0) {
        _mediaList[index]->setCurrIndex(Order); // "叫地主"
    }
    else if(point == 0) {
        if(isFirst) {
            _mediaList[index]->setCurrIndex(NoOrder); // "不叫"
        }
        else {
            _mediaList[index]->setCurrIndex(NoRob); // "不抢"
        }
    }
    else if(point == 2) {
        _mediaList[index]->setCurrIndex(Rob1); // "抢地主"
    }
    else if(point == 3) {
        _mediaList[index]->setCurrIndex(Rob2); // "我抢"
    }
    _mediaPlayer[index]->setSource(_mediaList[index]->currMedia());
    _mediaPlayer[index]->play();
    qDebug() << "调用BGMControl::playerRobLordMusic()成功";
}

void BGMControl::playCardMusic(Cards cards, bool isFirst, RoleSex sex) {
    qDebug() << "开始调用BGMControl::playCardMusic()";
    // 获取音频列表
    int index = (sex == Man) ? 0 : 1;
    MediaPlayList* mediaList = _mediaList[index];

    Card::CardPoint point = Card::Card_Begin;
    // 获取cards的牌型, 然后进行判断
    PlayHand hand(cards);
    PlayHand::HandType type = hand.getHandType();
    if(type == PlayHand::Hand_Single || type == PlayHand::Hand_Pair || type == PlayHand::Hand_Triple) {
        point = cards.takeRandomCard().point();
    }
    int number = 0; // 当前牌型对应到CardType的取值

    switch (type) {
    case PlayHand::Hand_Single: // 单牌
    {
        number = point - 1;
        break;
    }
    case PlayHand::Hand_Pair: // 对牌
    {
        number = point - 1 + 15; // +15: 跳过BGMControl::CardType的单张牌
        break;
    }
    case PlayHand::Hand_Triple: // 三张点数相同的牌
    {
        number = point - 1 + 15 + 13; // 又+13: 跳过BGMControl::CardType的对子
        break;
    }
    case PlayHand::Hand_Triple_Single: // 三带一
    {
        number = ThreeBindOne;
        break;
    }
    case PlayHand::Hand_Triple_Pair: // 三带二
    {
        number = ThreeBindPair;
        break;
    }
    case PlayHand::Hand_Plane:            // 飞机
    case PlayHand::Hand_Plane_Two_Single: // 飞机带两单
    case PlayHand::Hand_Plane_Two_Pair:   // 飞机带两对
    {
        number = Plane;
        break;
    }
    case PlayHand::Hand_Seq_Pair: // 连对
    {
        number = SequencePair;
        break;
    }
    case PlayHand::Hand_Seq_Single: // 顺子
    {
        number = Sequence;
        break;
    }
    case PlayHand::Hand_Bomb: // 炸弹
    {
        number = Bomb;
        break;
    }
    case PlayHand::Hand_Bomb_Jokers: // 王炸
    {
        number = JokerBomb;
        break;
    }
    case PlayHand::Hand_Bomb_Pair:              // 炸弹带一对
    case PlayHand::Hand_Bomb_Two_Single:        // 炸弹带两单
    case PlayHand::Hand_Bomb_Jokers_Pair:       // 王炸带一对
    case PlayHand::Hand_Bomb_Jokers_Two_Single: // 王炸带两单
    {
        number = FourBindTwo;
        break;
    }
    default:
        break;
    }

    if(!isFirst && (number >= Plane && number <= FourBindTwo)) { // 不是第一个出牌
        // 选择MoreBiger1或MoreBiger2
        mediaList->setCurrIndex(MoreBiger1 + QRandomGenerator::global()->bounded(2));
    }
    else { mediaList->setCurrIndex(number); }

    // 播放音乐
    _mediaPlayer[index]->setSource(mediaList->currMedia());
    _mediaPlayer[index]->play();
    if(number == Bomb || number == JokerBomb) {
        playAssintMusic(BombVoice);
    }
    if(number == Plane) {
        playAssintMusic(PlaneVoice);
    }
    qDebug() << "调用BGMControl::playCardMusic()成功";
}

void BGMControl::playLastMusic(CardType type, RoleSex sex) {
    qDebug() << "开始调用BGMControl::playLastMusic()";
    // 根据玩家的性别, 获取对应的播放列表
    int index = (sex == Man) ? 0 : 1;
    MediaPlayList* mediaList = _mediaList[index];
    if(_mediaPlayer[index]->playbackState() == QMediaPlayer::StoppedState) {
        mediaList->setCurrIndex(type);
        _mediaPlayer[index]->setSource(mediaList->currMedia());
        _mediaPlayer[index]->play();
    }
    else {
        QTimer::singleShot(1500, this, [=](){
            mediaList->setCurrIndex(type);
            _mediaPlayer[index]->setSource(mediaList->currMedia());
            _mediaPlayer[index]->play();
        });
    }
    qDebug() << "调用BGMControl::playLastMusic()成功";
}

void BGMControl::playPassMusic(RoleSex sex) {
    qDebug() << "开始调用BGMControl::playPassMusic()";
    // 根据玩家的性别, 获取对应的播放列表
    int index = (sex == Man) ? 0 : 1;
    MediaPlayList* mediaList = _mediaList[index];

    // 找到要播放的音乐
    int random = QRandomGenerator::global()->bounded(4);
    mediaList->setCurrIndex(Pass1 + random);

    // 播放音乐
    _mediaPlayer[index]->setSource(mediaList->currMedia());
    _mediaPlayer[index]->play();
    qDebug() << "调用BGMControl::playPassMusic()成功";
}

void BGMControl::playAssintMusic(AssistMusic type) {
    qDebug() << "开始调用BGMControl::playAssintMusic()";
    // 获取对应的播放列表和播放器
    MediaPlayList* mediaList = _mediaList[3];
    QMediaPlayer* mediaPlayer = _mediaPlayer[3];

    // 发牌需要虚循环播放
    if(type == Dispatch) { // 循环播放
        mediaPlayer->setLoops(QMediaPlayer::Infinite);
    }
    else { // 播放一次
        mediaPlayer->setLoops(QMediaPlayer::Once);
    }

    // 找到要播放的音乐
    mediaList->setCurrIndex(type);

    // 播放音乐
    mediaPlayer->setSource(mediaList->currMedia());
    mediaPlayer->play();
    qDebug() << "调用BGMControl::playAssintMusic()成功";
}

void BGMControl::stopAssintMusic() {
    _mediaPlayer[3]->stop();
}

void BGMControl::playEndingMusic(bool isWin) {
    if(isWin) {
        _mediaList[4]->setCurrIndex(0);
    }
    else {
        _mediaList[4]->setCurrIndex(1);
    }

    _mediaPlayer[4]->setSource(_mediaList[4]->currMedia());
    _mediaPlayer[4]->play();
}








