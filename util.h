#ifndef UTIL_H
#define UTIL_H
#include <QLabel>
#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

/************************ constants *******************/

const char BLACKCHESS[] = "./images/stoneB.png";
const char WHITECHESS[] = "./images/stoneW.png";
const char HINT[] = "./images/hint.png";
const char BOARD_1[] = "./images/chessboard.png";
const char BOARD_2[] = "./images/board2.png";
const char BORDER[] = "./images/border.png";
const char LASTMOVE[] = "./images/lastmove.png";

//#define _MY_DEBUG

#ifdef _MY_DEBUG
const QString SERVER_IP = "127.0.0.1";
#else
const QString SERVER_IP = "149.28.93.3";
#endif
/******************************************************/

void setPicture(QLabel* label, const char dir[]);

class Notice:public QWidget {
public:
    Notice(QWidget* parent);
    void display(const QString& text);
    ~Notice();
private:
    QGraphicsOpacityEffect* effect;
    QPropertyAnimation *animation;
    QLabel* label;
};



#endif // UTIL_H
