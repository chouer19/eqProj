#include "mainwindow.h"
#include <QApplication>
#include<QDateTime>
#include<QFontDatabase>



int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    //设置DroidSansFallback.ttf字体为程序默认字体,将字体文件打包进apk
    int fontId = QFontDatabase::addApplicationFont(":/new/image/msyh.ttc");
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    QString androidFont = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(androidFont);
    QGuiApplication::setFont(font);
    MainWindow w;
//    w.setWindowFlags(w.windowFlags()|Qt::WindowStaysOnTopHint);
    w.show();
//    w.showFullScreen();


    return a.exec();


}


