#ifndef MYITEM_H
#define MYITEM_H
#include <QWidget>
#include <QLabel>
#include <QLayout>

class MyItem : public QWidget{
    QGridLayout layout;
    QLabel Type, black, bName, white, wName;
public:
    MyItem(int _type, QString _bName, QString _wName);
    int type;
    //uid is a 4-digit int used to uniquely identify a game.
    int uid;
    QString nameb, namew;
};

#endif // MYITEM_H
