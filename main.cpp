#include "gamepanel.h"
#include "loading.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 解决某些版本的Qt机器人线程无法执行的bug
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    // QResource::registerResource("./resource.rcc");
    Loading w;
    w.show();
    return a.exec();
}
