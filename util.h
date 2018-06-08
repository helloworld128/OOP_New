#ifndef UTIL_H
#define UTIL_H
#include <QLabel>
#include <QWidget>
#include <QGraphicsOpacityEffect>

/************************ constants *******************/

const char BLACKCHESS[] = "./images/stoneB.png";
const char WHITECHESS[] = "./images/stoneW.png";
const char HINT[] = "./images/hint.png";
const char BOARD_1[] = "./images/chessboard.png";
const char BOARD_2[] = "./images/board2.png";
const char BORDER[] = "./images/border.png";
//enum TYPE{REVERSI, FIR, GO};
/******************************************************/

void setPicture(QLabel* label, const char dir[]);

class Notice:public QWidget{

public:
    Notice(QWidget* parent);
    QGraphicsOpacityEffect* effect;
    QLabel* label;
    ~Notice();
};



#endif // UTIL_H
