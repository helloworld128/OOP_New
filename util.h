#ifndef UTIL_H
#define UTIL_H
#include <QLabel>

const char BLACKCHESS[] = "./images/stoneB.png";
const char WHITECHESS[] = "./images/stoneW.png";
const char HINT[] = "./images/hint.png";
const char BOARD_1[] = "./images/chessboard.png";
const char BOARD_2[] = "./images/board2.png";
const char BORDER[] = "./images/border.png";

void setPicture(QLabel* label, const char dir[]);



#endif // UTIL_H
