#include "gamepanel.h"
#include "loading.h"

#include <QApplication>
#include <QResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 解决某些版本的Qt机器人线程无法执行的bug
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    // 在程序中注册二进制格式的资源文件
    QResource::registerResource("./resource.rcc"); // 给当前项目指定资源文件
    Loading w;
    w.show();
    return a.exec();
}
