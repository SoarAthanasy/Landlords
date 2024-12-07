#include "mediaplaylist.h"

MediaPlayList::MediaPlayList(QObject *parent) : QObject{parent} {}

void MediaPlayList::addMedia(const QUrl& Url) {
    _mediaList.push_back(Url);
}

void MediaPlayList::clear() { _mediaList.clear(); }

void MediaPlayList::setCurrIndex(int index) {
    if(index >= _mediaList.size()) {
        qDebug() << "传入MediaPlayList::setCurrIndex(int index)的index非法";
        return; // index非法, 不修改_currIndex;
    }
    _currIndex = index;
}
int MediaPlayList::getCurrIndex() { return _currIndex; }

QUrl MediaPlayList::operator[](int index) {
    if(index >= _mediaList.size()) {
        qDebug() << "传入MediaPlayList::operator[](int index)的index非法";
        return QUrl(); // index非法, 返回空对象
    }
    return _mediaList[index];
}

QUrl MediaPlayList::currMedia() { return _mediaList[_currIndex]; }

int MediaPlayList::size() const { return _mediaList.size(); }
