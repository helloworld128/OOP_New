#include "widget.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load("./zh_cn.qm");
    a.installTranslator(&translator);
    Widget* w = new Widget;
    w->show();

    return a.exec();
}
