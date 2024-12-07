#ifndef MEDIAPLAYLIST_H
#define MEDIAPLAYLIST_H

#include <QObject>
#include <QUrl>
#include <QDebug>

class MediaPlayList : public QObject {
    Q_OBJECT
public:
    explicit MediaPlayList(QObject *parent = nullptr);

    // 修改列表
    void addMedia(const QUrl& Url); // 添加音乐到播放列表
    void clear();                   // 清空播放列表

    // 索引
    void setCurrIndex(int index);
    int getCurrIndex();

    // 获取Url
    QUrl operator[](int index); // 获取索引index对应的Url
    QUrl currMedia();           // 获取当前索引对应的Url

    int size() const;
signals:
    void currentmMediaChanged(const QUrl& Url); // 发送当前要播放的媒体信号
private:
    QVector<QUrl> _mediaList; // 媒体列表
    int _currIndex = 0;       // 媒体列表的当前索引
};

#endif // MEDIAPLAYLIST_H
