#include "myitem.h"
#include <QLabel>

MyItem::MyItem(int _type, QString _bName, QString _wName)
{
    switch (_type){
    case 0:
        type.setText("Reversi");
        break;
    case 1:
        type.setText("FiveInaRow");
        break;
    case 2:
        type.setText("Go");
        break;
    }
    layout.addWidget(&type,1,1,1,2);
    bName.setText(_bName); wName.setText(_wName);
    black.setText("black:"); white.setText("white:");
    layout.addWidget(&black,2,1); layout.addWidget(&bName,2,2);
    layout.addWidget(&white,3,1); layout.addWidget(&wName,3,2);
    setLayout(&layout);
}

